======== Summary =========

This tool generates data that can be used to demonstrate packet spoofing
in the Source engine. It should not be used maliciously.

======= Parameters =======

out_seq:

- Outgoing sequence value
- Needs to be higher than the target's actual sequence
- Must be below 0xFFFFFFFE

inc_seq:

- Incoming sequence value
- Needs to be higher than the target's actual sequence

message:

- Type of message to generate
- Only net_stringcmd is supported by default

command:

- Command to write in net_stringcmd message