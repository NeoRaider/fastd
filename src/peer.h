/*
  Copyright (c) 2012-2014, Matthias Schiffer <mschiffer@universe-factory.net>
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
   \file

   Structures and functions for peer management
*/


#pragma once

#include "fastd.h"


/** The state of a peer */
typedef enum fastd_peer_state {
	STATE_INIT = 0,					/**< The peer peer was just created */
	STATE_RESOLVING,				/**< The peer is currently resolving its first remote */
	STATE_HANDSHAKE,				/**< The peer has tried to perform a handshake */
	STATE_ESTABLISHED,				/**< The peer has established a connection */
} fastd_peer_state_t;

/** Dynamic state of a peer */
struct fastd_peer {
	uint64_t id;					/**< A unique ID assigned to each peer */

	fastd_peer_config_t *config;			/**< The peer's fastd_peer_config_t */

	/** The socket used by the peer. This can either be a common bound socket or a
	    dynamic, unbound socket that is used exclusively by this peer */
	fastd_socket_t *sock;
	fastd_peer_address_t local_address;		/**< The local address used to communicate with this peer */
	fastd_peer_address_t address;			/**< The peers current address */

	fastd_peer_state_t state;			/**< The peer's state */
	struct timespec timeout;			/**< The timeout after which the peer is reset */
	struct timespec keepalive_timeout;		/**< The timeout after which a keepalive is sent to the peer */

	VECTOR(fastd_remote_t) remotes;			/**< The vector of the peer's remotes */
	ssize_t next_remote;				/**< An index into the field remotes or -1 */

	struct timespec next_handshake;			/**< The time of the next handshake */
	fastd_dlist_head_t handshake_entry;		/**< Entry in the handshake queue */

	struct timespec last_handshake_timeout;		/**< No handshakes are sent to the peer until this timeout has occured to avoid flooding the peer */
	fastd_peer_address_t last_handshake_address;	/**< The address the last handshake was sent to */

	struct timespec last_handshake_response_timeout; /**< All handshakes from last_handshake_address will be ignored until this timeout has occured */
	fastd_peer_address_t last_handshake_response_address; /**< The address the last handshake was received from */

	struct timespec establish_handshake_timeout;	/**< A timeout during which all handshakes for this peer will be ignored after a new connection has been established */

#ifdef WITH_DYNAMIC_PEERS
	bool dynamic;					/**< Specifies if the peer has been added dynamically by a on-verify script */

	struct timespec verify_timeout;			/**< Specifies the minimum time after which on-verify may be run again */
	struct timespec verify_valid_timeout;		/**< Specifies how long a peer stays valid after a successful on-verify run */
#endif

	fastd_protocol_peer_state_t *protocol_state;	/**< Protocol-specific peer state */
};

/**
   Static configuration of a peer

   Peer configurations are kept in a linked list.
*/
struct fastd_peer_config {
	fastd_peer_config_t *next;			/**< The next peer configuration */

	const char *config_source_dir;			/**< The directory this peer's configuration was loaded from */

	bool enabled;					/**< Specifies if this peer was disabled because of a configuration error */
	char *name;					/**< The peer's name */

	fastd_remote_config_t *remotes;			/**< A linked list of the peer's remote entries */
	char *key;					/**< The peer's public key */
	bool floating;					/**< Specifies if the peer has any floating remotes */
	const fastd_peer_group_t *group;		/**< The peer group the peer belongs to */

	fastd_protocol_peer_config_t *protocol_config;	/**< The protocol-specific configuration of the peer */
};

/**
   A group of peers

   Peer groups may be nested and form a tree
*/
struct fastd_peer_group {
	fastd_peer_group_t *next;			/**< The next sibling in the group tree */
	fastd_peer_group_t *parent;			/**< The group's parent group */
	fastd_peer_group_t *children;			/**< The group's first child */

	char *name;					/**< The group's name; NULL for the root group */
	fastd_string_stack_t *peer_dirs;		/**< List of peer directories which belong to this group */

	/* constraints */
	int max_connections;				/**< The maximum number of connections to allow in this group; -1 for no limit */
};

/** An entry for a MAC address seen at another peer */
struct fastd_peer_eth_addr {
	fastd_eth_addr_t addr;				/**< The MAC address */
	fastd_peer_t *peer;				/**< The corresponding peer */
	struct timespec timeout;			/**< Timeout after which the address entry will be purged */
};

/** A resolved remote entry */
struct fastd_remote {
	fastd_remote_config_t *config;			/**< The remote's configuration */

	size_t n_addresses;				/**< The size of the \e addresses array */
	size_t current_address;				/**< The index of the remote the next handshake will be sent to */
	fastd_peer_address_t *addresses;		/**< The IP addresses the remote was resolved to */

	struct timespec last_resolve_timeout;		/**< Timeout before the remote must not be resolved again */
};

/** An address or hostname entry associated with a peer */
struct fastd_remote_config {
	fastd_remote_config_t *next;			/**< The next remote for the peer */

	char *hostname;					/**< The hostname or NULL */
	fastd_peer_address_t address;			/**< The address; if hostname is set only sin.sin_port is used */
};


bool fastd_peer_address_equal(const fastd_peer_address_t *addr1, const fastd_peer_address_t *addr2);
void fastd_peer_address_simplify(fastd_peer_address_t *addr);
void fastd_peer_address_widen(fastd_peer_address_t *addr);

/** Returns the port of a fastd_peer_address_t (in network byte order) */
static inline uint16_t fastd_peer_address_get_port(const fastd_peer_address_t *addr) {
	switch (addr->sa.sa_family) {
	case AF_INET:
		return addr->in.sin_port;

	case AF_INET6:
		return addr->in6.sin6_port;

	default:
		return 0;
	}
}

fastd_peer_config_t* fastd_peer_config_new(fastd_peer_group_t *group);
void fastd_peer_config_free(fastd_peer_config_t *peer);
void fastd_peer_config_purge(fastd_peer_config_t *config);
bool fastd_peer_config_equal(const fastd_peer_config_t *peer1, const fastd_peer_config_t *peer2);

void fastd_peer_reset(fastd_peer_t *peer);
void fastd_peer_delete(fastd_peer_t *peer);
fastd_peer_t* fastd_peer_add(fastd_peer_config_t *peer_conf);
void fastd_peer_set_established(fastd_peer_t *peer);
bool fastd_peer_may_connect(fastd_peer_t *peer);
void fastd_peer_handle_resolve(fastd_peer_t *peer, fastd_remote_t *remote, size_t n_addresses, const fastd_peer_address_t *addresses);
bool fastd_peer_owns_address(const fastd_peer_t *peer, const fastd_peer_address_t *addr);
bool fastd_peer_matches_address(const fastd_peer_t *peer, const fastd_peer_address_t *addr);
bool fastd_peer_claim_address(fastd_peer_t *peer, fastd_socket_t *sock, const fastd_peer_address_t *local_addr, const fastd_peer_address_t *remote_addr, bool force);
void fastd_peer_reset_socket(fastd_peer_t *peer);
void fastd_peer_schedule_handshake(fastd_peer_t *peer, int delay);
fastd_peer_t* fastd_peer_find_by_id(uint64_t id);

void fastd_peer_set_shell_env(fastd_shell_env_t *env, const fastd_peer_t *peer, const fastd_peer_address_t *local_addr, const fastd_peer_address_t *peer_addr);
void fastd_peer_exec_shell_command(const fastd_shell_command_t *command, const fastd_peer_t *peer, const fastd_peer_address_t *local_addr, const fastd_peer_address_t *peer_addr);

/**
   Schedules a handshake with the default delay and jitter

   The default relay is between 17.5 and 22.5 seconds
*/
static inline void fastd_peer_schedule_handshake_default(fastd_peer_t *peer) {
	fastd_peer_schedule_handshake(peer, fastd_rand(17500, 22500));
}

/** Cancels a scheduled handshake */
static inline void fastd_peer_unschedule_handshake(fastd_peer_t *peer) {
	fastd_dlist_remove(&peer->handshake_entry);
}

#ifdef WITH_DYNAMIC_PEERS
/** Call to signal that there is currently an asychronous on-verify command running for the peer */
static inline void fastd_peer_set_verifying(fastd_peer_t *peer) {
	peer->verify_timeout = fastd_in_seconds(MIN_VERIFY_INTERVAL);
}

/** Marks the peer verification as successful or failed */
static inline void fastd_peer_set_verified(fastd_peer_t *peer, bool ok) {
	peer->verify_valid_timeout = ok ? fastd_in_seconds(VERIFY_VALID_TIME) : ctx.now;
}
#endif

/** Checks if there's a handshake queued for the peer */
static inline bool fastd_peer_handshake_scheduled(fastd_peer_t *peer) {
	return fastd_dlist_linked(&peer->handshake_entry);
}

/** Checks if a peer config has floating remotes (or no remotes at all) */
static inline bool fastd_peer_config_is_floating(const fastd_peer_config_t *config) {
	return (!config->remotes || config->floating);
}

/** Checks if a peer is floating (is has at least one floating remote or no remotes at all) */
static inline bool fastd_peer_is_floating(const fastd_peer_t *peer) {
	return peer->config ? fastd_peer_config_is_floating(peer->config) : true;
}

/** Checks if a peer is not statically configured, but added after a on-verify run */
static inline bool fastd_peer_is_dynamic(const fastd_peer_t *peer UNUSED) {
#ifdef WITH_DYNAMIC_PEERS
	return peer->dynamic;
#else
	return false;
#endif
}

/** Returns the currently active remote entry */
static inline fastd_remote_t * fastd_peer_get_next_remote(fastd_peer_t *peer) {
	if (peer->next_remote < 0)
	     return NULL;

	return &VECTOR_INDEX(peer->remotes, peer->next_remote);
}

/** Checks if the peer currently has an established connection */
static inline bool fastd_peer_is_established(const fastd_peer_t *peer) {
	switch(peer->state) {
	case STATE_ESTABLISHED:
		return true;

	default:
		return false;
	}
}

/** Returns the peer's peer group */
static inline const fastd_peer_group_t * fastd_peer_get_group(const fastd_peer_t *peer) {
	if (peer->config)
		return peer->config->group;
	else
		return conf.peer_group;
}

/** Signals that a valid packet was received from the peer */
static inline void fastd_peer_seen(fastd_peer_t *peer) {
	peer->timeout = fastd_in_seconds(PEER_STALE_TIME);
}

/** Checks if a peer uses dynamic sockets (which means that each connection attempt uses a new socket) */
static inline bool fastd_peer_is_socket_dynamic(const fastd_peer_t *peer) {
	return (!peer->sock || !peer->sock->addr);
}

/** Checks if a MAC address is a normal unicast address */
static inline bool fastd_eth_addr_is_unicast(fastd_eth_addr_t addr) {
	return ((addr.data[0] & 1) == 0);
}

void fastd_peer_eth_addr_add(fastd_peer_t *peer, fastd_eth_addr_t addr);
fastd_peer_t* fastd_peer_find_by_eth_addr(fastd_eth_addr_t addr);

void fastd_peer_handle_handshake_queue(void);
void fastd_peer_maintenance(void);
