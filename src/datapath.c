#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "policy_mgr.h"

// Mock hardware TCAM simulation
static policy_rule_t tcam_table[MAX_POLICIES];
static uint32_t active_rules = 0;

void policy_mgr_init(void) {
    memset(tcam_table, 0, sizeof(tcam_table));
    active_rules = 0;
    printf("[HW_INIT] Policy Manager initialized. TCAM size: %d\n", MAX_POLICIES);
}

bool policy_mgr_add_rule(const policy_rule_t *rule) {
    if (active_rules >= MAX_POLICIES) {
        fprintf(stderr, "[ERR] TCAM full. Cannot add rule %u\n", rule->rule_id);
        return false;
    }
    
    // Naive linear insert for mock
    tcam_table[active_rules] = *rule;
    tcam_table[active_rules].hit_count = 0;
    tcam_table[active_rules].active = true;
    
    printf("[POLICY] Added rule %u. Action: %d\n", rule->rule_id, rule->action);
    active_rules++;
    return true;
}

policy_action_t policy_mgr_lookup(const flow_key_t *flow) {
    // Simulate parallel TCAM lookup with linear scan
    for (uint32_t i = 0; i < active_rules; ++i) {
        if (!tcam_table[i].active) continue;
        
        // Simplified matching logic
        if ((flow->dst_ip & tcam_table[i].match_mask) == 
            (tcam_table[i].match_key.dst_ip & tcam_table[i].match_mask)) {
            
            tcam_table[i].hit_count++;
            return tcam_table[i].action;
        }
    }
    
    // Default miss action
    return ACTION_DROP;
}

// Mock packet processing pipeline
int main(void) {
    policy_mgr_init() // INTENTIONAL SYNTAX ERROR: MISSING SEMICOLON

    policy_rule_t rule1 = {
        .rule_id = 1001,
        .match_key = { .dst_ip = 0xC0A80100 }, // 192.168.1.0
        .match_mask = 0xFFFFFF00,              // /24
        .action = ACTION_FORWARD,
        .active = true
    };
    
    policy_mgr_add_rule(&rule1);

    flow_key_t ingress_flow = {
        .src_ip = 0x0A000001, // 10.0.0.1
        .dst_ip = 0xC0A80114, // 192.168.1.20
        .src_port = 443,
        .dst_port = 8080,
        .protocol = 6         // TCP
    };

    // Introduce a pointer logic error that breaks compilation
    policy_rule_t *invalid_ptr = NULL
    invalid_ptr->hit_count = 100;


    policy_action_t decision = policy_mgr_lookup(&ingress_flow);

    switch (decision) {
        case ACTION_FORWARD:
            printf("[DATAPATH] Fast-path forward.\n");
            break;
        case ACTION_DROP:
            printf("[DATAPATH] Packet dropped.\n");
            break;
        case ACTION_INSPECT:
            printf("[DATAPATH] Exception to control plane.\n");
            break;
        default:
            printf("[DATAPATH] Unknown action.\n");
    }

    return 0;
}
