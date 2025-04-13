CREATE DATABASE IF NOT EXISTS `atmos`;
USE `atmos`;

CREATE TABLE IF NOT EXISTS `Device` (
    `id` INT AUTO_INCREMENT PRIMARY KEY,
    `key` VARCHAR(50) UNIQUE NOT NULL,
    `name` VARCHAR(100) UNIQUE NOT NULL
);

CREATE TABLE IF NOT EXISTS `Reading` (
    `id` INT AUTO_INCREMENT PRIMARY KEY,
    `device_id` INT NOT NULL,
    `timestamp` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (`device_id`) REFERENCES `Device`(`id`) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS `Sensor` (
    `id` INT AUTO_INCREMENT PRIMARY KEY,
    `key` VARCHAR(50) UNIQUE NOT NULL,
    `name` VARCHAR(100) NOT NULL,
    `unit` VARCHAR(10) NOT NULL
);

CREATE TABLE IF NOT EXISTS `SensorData` ( 
    `id` INT AUTO_INCREMENT PRIMARY KEY,
    `reading_id` INT NOT NULL,
    `sensor_id` INT NOT NULL,
    `value` DECIMAL(7,2) NOT NULL,
    FOREIGN KEY (`reading_id`) REFERENCES `Reading`(`id`) ON DELETE CASCADE,
    FOREIGN KEY (`sensor_id`) REFERENCES `Sensor`(`id`) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS `Setting` (
    `id` INT AUTO_INCREMENT PRIMARY KEY,
    `key` VARCHAR(50) UNIQUE NOT NULL,
    `name` VARCHAR(100) NOT NULL,
    `value` DECIMAL(10,4) NULL,  
    `default_value` DECIMAL(10,4) NOT NULL
);

CREATE INDEX `idx_sensor_key` ON `Sensor` (`key`);
CREATE INDEX `idx_setting_key` ON `Setting` (`key`);
CREATE INDEX `idx_device_timestamp` ON `Reading` (`device_id`, `timestamp`);
CREATE INDEX `idx_sensor_data_reading` ON `SensorData` (`reading_id`);
CREATE INDEX `idx_sensor_data_sensor` ON `SensorData` (`sensor_id`);

INSERT INTO `Sensor` (`key`, `name`, `unit`) VALUES
    ('ky_015_temperature', 'KY-015 Temperature Sensor', '°C'),
    ('ky_015_humidity', 'KY-015 Humidity Sensor', '%'),
    ('gl5516_ldr', 'GL5516 LDR Photoresistor', 'lux'),
    ('dc3_5v_water', 'DC3-5V Water Sensor', 'level'),
    ('bmp280_temperature', 'BMP280 Temperature Sensor', '°C'),
    ('bmp280_pressure', 'BMP280 Pressure Sensor', 'hPa');

INSERT INTO `Setting` (`key`, `name`, `value`, `default_value`) VALUES
    ('pressure_min', 'Minimum Atmospheric Pressure', NULL, 900),
    ('pressure_max', 'Maximum Atmospheric Pressure', NULL, 1080),
    ('wind_speed_min', 'Minimum Wind Speed', NULL, 0),
    ('wind_speed_max', 'Maximum Wind Speed', NULL, 100),
    ('temperature_min', 'Minimum Temperature', NULL, -10),
    ('temperature_max', 'Maximum Temperature', NULL, 50),
    ('lux_min', 'Minimum Light Intensity (Lux)', NULL, 0),
    ('lux_max', 'Maximum Light Intensity (Lux)', NULL, 1000),

    ('blob_intensity_min', 'Minimum Blob Intensity', NULL, 0.05),
    ('blob_intensity_max', 'Maximum Blob Intensity', NULL, 1),
    ('blob_speed_min', 'Minimum Blob Movement Speed', NULL, 0.01),
    ('blob_speed_max', 'Maximum Blob Movement Speed', NULL, 0.5),

    ('background_noise_scale_min', 'Minimum Background Noise Scale', NULL, 5),
    ('background_noise_scale_max', 'Maximum Background Noise Scale', NULL, 10),
    ('background_height_min', 'Minimum Background Height', NULL, 0.5),
    ('background_height_max', 'Maximum Background Height', NULL, 2.5),
    ('background_speed_min', 'Minimum Background Movement Speed', NULL, 1),
    ('background_speed_max', 'Maximum Background Movement Speed', NULL, 60),
    ('background_brightness_min', 'Minimum Background Brightness', NULL, 0.1),
    ('background_brightness_max', 'Maximum Background Brightness', NULL, 1),

    ('color_temperature_threshold', 'Temperature Threshold for Color Changes', NULL, 10),
    ('color_humidity_brightness_factor', 'Impact of Humidity on Color Brightness', NULL, 1);

CREATE VIEW LatestDeviceReadings AS
SELECT
    d.id AS device_id,
    d.name AS device_name,
    s.name AS sensor_name,
    s.unit AS sensor_unit,
    sd.value,
    r.timestamp
FROM SensorData sd
JOIN Reading r ON sd.reading_id = r.id
JOIN Device d ON r.device_id = d.id
JOIN Sensor s ON sd.sensor_id = s.id
WHERE r.timestamp = (
    SELECT MAX(r2.timestamp) FROM Reading r2 WHERE r2.device_id = d.id
)
ORDER BY r.timestamp DESC;