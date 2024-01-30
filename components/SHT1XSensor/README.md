# SHT1x ESPHome Sensor

This is an ESPHome component for the SHT1x temperature and humidity sensor. It allows you to easily integrate the SHT1x sensor into your ESPHome projects.

## Installation

To use this component, you need to add the `SHT1XSensor` component from the `landonr/esphome-components` GitHub repository to your ESPHome configuration.

### Example 

```yaml
external_components:
  - source: github://landonr/esphome-components
    components: [ SHT1XSensor ]

SHT1XSensor:
  update_interval: 10s
  data_pin: 22
  clock_pin: 23
  humidity: 
    id: humidity_sensor
    internal: false
  temperature: 
    id: temperature_sensor
    internal: false
```