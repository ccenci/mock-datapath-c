#ifndef POLICY_MGR_H
#define POLICY_MGR_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_POLICIES 256
#define MAX_FLOWS 1024

typedef enum {
    ACTION_DROP = 0,
    ACTION_FORWARD,
    ACTION_MIRROR,
    ACTION_INSPECT
} policy_action_t;

typedef struct {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
} flow_key_t;

typedef struct {
    uint32_t rule_id;
    flow_key_t match_key;
    uint32_t match_mask; 
    policy_action_t action;
    uint32_t hit_count;
    bool active;
} policy_rule_t;

void policy_mgr_init(void);
policy_action_t policy_mgr_lookup(const flow_key_t *flow);
bool policy_mgr_add_rule(const policy_rule_t *rule);

#endif // POLICY_MGR_H
