package tema.meteorolog.controllers

import org.slf4j.Logger
import org.slf4j.LoggerFactory
import org.springframework.http.HttpStatus
import org.springframework.http.ResponseEntity
import org.springframework.web.bind.annotation.*
import tema.meteorolog.controllers.dto.RequestDto
import tema.meteorolog.entries.DeviceEntry
import tema.meteorolog.entries.WeatherEntry
import tema.meteorolog.repositories.DeviceRepository
import tema.meteorolog.repositories.WeatherRepository
import java.time.Instant
import java.time.LocalDateTime
import java.time.ZoneOffset
import java.time.format.DateTimeFormatter

@RestController
@CrossOrigin(maxAge = 3600)
@RequestMapping("/api")
class TemperatureReadingsController(
    private val deviceRepository: DeviceRepository,
    private val weatherRepository: WeatherRepository
) {
    private val log: Logger = LoggerFactory.getLogger(this::class.java)

    @GetMapping("/readings")
    fun getAllReadings(): ResponseEntity<List<WeatherEntry>> {
        log.info("Invoke getAllReadings")
        return try {
            val weatherEntries = weatherRepository.findAll()

            if (weatherEntries.isEmpty()) {
                ResponseEntity(HttpStatus.NO_CONTENT)
            } else ResponseEntity(weatherEntries, HttpStatus.OK)
        } catch (e: Exception) {
            ResponseEntity(null, HttpStatus.INTERNAL_SERVER_ERROR)
        }
    }


    @PostMapping("/reading")
    fun postReading(@RequestBody requestData: RequestDto
    ) : ResponseEntity<Any> {
        log.info("Message received! $requestData")
        val device = deviceRepository.findBySerialNumber(requestData.serialNumber)
        if (device != null) {
            val weatherEntry = WeatherEntry(
                id = null,
                time = Instant.now(),
                receiveTime = parseTime(requestData.time),
                device = device,
                temp = requestData.temperature.toDouble(),
                pressure = requestData.pressure.toDouble(),
                altitude = requestData.altitude.toDouble()
            )
            weatherRepository.save(weatherEntry)
        } else {
            log.error("Can't find device with serial number ${requestData.serialNumber}")
        }
        return ResponseEntity(HttpStatus.OK)
    }

    private fun parseTime(dateTimeMessage: String): Instant {
        val localDateTime = LocalDateTime.parse(dateTimeMessage, DateTimeFormatter.ofPattern("dd.MM.yyyy HH:mm:ss"))
        return  localDateTime.toInstant(ZoneOffset.of("Europe/Moscow"))
    }

}