-- Create the Database
CREATE DATABASE IF NOT EXISTS `atmos`;
USE `atmos`;

-- Create Devices Table
CREATE TABLE IF NOT EXISTS `Device` (
    `id` INT AUTO_INCREMENT PRIMARY KEY,
    `name` VARCHAR(100) UNIQUE NOT NULL
);

-- Create Readings Table
CREATE TABLE IF NOT EXISTS `Reading` (
    `id` INT AUTO_INCREMENT PRIMARY KEY,
    `device_id` INT NOT NULL,
    `timestamp` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (`device_id`) REFERENCES `Device`(`id`) ON DELETE CASCADE
);

-- Create Sensor Data Table
CREATE TABLE IF NOT EXISTS `SensorData` (
    `id` INT AUTO_INCREMENT PRIMARY KEY,
    `reading_id` INT NOT NULL,
    `sensor_type` VARCHAR(50) NOT NULL,  
    `sensor_category` ENUM('temperature', 'humidity', 'pressure', 'light', 'wind') NOT NULL,
    `value` DECIMAL(7,2) NOT NULL,
    `unit` VARCHAR(10) NOT NULL,  
    FOREIGN KEY (`reading_id`) REFERENCES `Reading`(`id`) ON DELETE CASCADE
);

-- Indexes for performance
CREATE INDEX `idx_device_timestamp` ON `Reading` (`device_id`, `timestamp`);
CREATE INDEX `idx_sensor_category` ON `SensorData` (`sensor_category`);

-- Create Views for easy querying
CREATE OR REPLACE VIEW `latestSensorReadings` AS
SELECT d.`name` AS `device_name`, r.`timestamp`, 
       s.`sensor_type`, s.`sensor_category`, s.`value`, s.`unit`
FROM `Reading` r
JOIN `Device` d ON r.`device_id` = d.`id`
JOIN `SensorData` s ON r.`id` = s.`reading_id`
WHERE r.`timestamp` = (SELECT MAX(`timestamp`) FROM `Reading` WHERE `Reading`.`device_id` = r.`device_id`);

CREATE OR REPLACE VIEW `deviceSensorHistory` AS
SELECT d.`name` AS `device_name`, r.`timestamp`, 
       s.`sensor_type`, s.`sensor_category`, s.`value`, s.`unit`
FROM `Reading` r
JOIN `Device` d ON r.`device_id` = d.`id`
JOIN `SensorData` s ON r.`id` = s.`reading_id`
ORDER BY r.`timestamp` DESC;

CREATE OR REPLACE VIEW `aggregatedSensorStatistics` AS
SELECT d.`name` AS `device_name`, 
       s.`sensor_category`, s.`sensor_type`,
       AVG(s.`value`) AS `avg_value`, s.`unit`,
       MIN(s.`value`) AS `min_value`, 
       MAX(s.`value`) AS `max_value`
FROM `SensorData` s
JOIN `Reading` r ON s.`reading_id` = r.`id`
JOIN `Device` d ON r.`device_id` = d.`id`
WHERE r.`timestamp` >= NOW() - INTERVAL 1 DAY
GROUP BY d.`name`, s.`sensor_category`, s.`sensor_type`, s.`unit`;
