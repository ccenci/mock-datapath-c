# Programmable Policy Manager (Mock)

This repository contains mock C code simulating a low-level Programmable Policy Manager architecture. 
It mimics datapath logic typical of specialized processors, including:
- Simulated hardware TCAM rule insertion.
- Fast-path flow lookups.
- Packet action enforcement (Forward, Drop, Inspect, Mirror).

## Structure
- `src/policy_mgr.h`: Core structures and constants for the policy engine.
- `src/datapath.c`: Packet processing pipeline and TCAM emulation logic.

## Compilation
```bash
make
./mock_datapath
```
