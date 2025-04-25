# SoC RAIL test

The RAILtest application (RAILtest) provides you with a simple tool for testing the radio and the functionality of the RAIL library. For more advanced usage, developers must write software against the RAIL library and create a custom radio configuration.


# Modes

RAILtest can be in a number of different modes. Each of these modes enables a subset of the test application's functionality, so that
nonsensical commands (such as trying to transmit a tone and a packet at the same time) are ignored. The following are supported modes,
with a short description.
None - The app is doing nothing, but the radio may be on. Parameters can be set in this mode based on the current radio state, and the
timer can be used.
Stream - Send a stream of pseudo-random bits.
Tone - Send a tone at the carrier frequency.
ContinuousTx - Send an unending stream of packets, separated by a configurable delay.
DirectMode - Send data to and from a GPIO, without any packet handling.
PacketTx - Send a specific number of packets, with a summary print at the end.
ScheduledTx - Send one packet scheduled in the future.
TxAfterRx - Schedule a packet after each RX after a specific delay.
RxOverflow - Overflow on receive by delaying in RAILCb_RxPacketReceived.
TxUnderflow - Underflow by not loading data for the next TX sequence.
TxCancel - Cancel a single packet transmit to force an error callback.
RfSense - Sense RF energy to wake the radio.
PER (Packet Error Rate) test - A GPIO is toggled, and stats are gathered on received packets.
BER (Bit Error Rate) test - Statistics are gathered on received bits.

---
## Module compatibility
---
1. B1BMBG22M3
2. B1BMBG22M6