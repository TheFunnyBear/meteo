package tema.meteorolog.controllers

import org.slf4j.Logger
import org.slf4j.LoggerFactory
import org.springframework.http.HttpStatus
import org.springframework.http.ResponseEntity
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RequestParam
import tema.meteorolog.entries.DeviceEntry
import tema.meteorolog.repositories.DeviceRepository

class DeviceController(
    private val deviceRepository: DeviceRepository
) {
    private val log: Logger = LoggerFactory.getLogger(this::class.java)

    @GetMapping("/devices")
    /**
     * Возвращает список устройств, возможна фильтрация результатов по наименованию устройства
     */
    fun getAllDevices(
        @RequestParam(required = false) deviceName: String?
    ): ResponseEntity<List<DeviceEntry>> {
        log.info("Invoke getAllDevices. deviceName = ${deviceName ?: "NA"}")
        return try {
            val devices = if (deviceName == null)
                deviceRepository.findAllOrdered()
                else
                deviceRepository.filterByDeviceNameOrdered(deviceName)

            if (devices.isEmpty()) {
                ResponseEntity(HttpStatus.NO_CONTENT)
            } else ResponseEntity(devices, HttpStatus.OK)
        } catch (e: Exception) {
            ResponseEntity(null, HttpStatus.INTERNAL_SERVER_ERROR)
        }
    }
}