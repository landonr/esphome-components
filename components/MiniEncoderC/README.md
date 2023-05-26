# M5Stack MiniEncoderC component

For M5Stick C and M5Stick C Plus

This component provides support for the [M5Stack MiniEncoderC](https://github.com/m5stack/M5Unit-MiniEncoderC)

You need an `i2c:` component configured.  All the sensors are optional.

Example:
```yaml
MiniEncoderC:
  id: encoder
  i2c_id: bus_b
  # address: 0x42
  button:
    name: Rotary Button
    id: rotary_button
  encoder:
    name: Rotary Encoder
    id: rotary
    encoder_filter: 2 // (1-100) divides the encoder value to make it less sensitive
```
