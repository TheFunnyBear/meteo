package tema.meteorolog

import org.slf4j.Logger
import org.slf4j.LoggerFactory
import org.springframework.boot.autoconfigure.SpringBootApplication
import org.springframework.boot.runApplication
import org.springframework.context.annotation.Configuration
import org.springframework.scheduling.annotation.EnableScheduling
import org.springframework.scheduling.annotation.Scheduled
import org.springframework.stereotype.Service
import tema.meteorolog.entries.DeviceEntry
import tema.meteorolog.entries.WeatherEntry
import tema.meteorolog.repositories.DeviceRepository
import tema.meteorolog.repositories.WeatherRepository
import java.lang.Math.random
import java.time.Instant
import java.util.*
import kotlin.math.log
import kotlin.random.Random

@SpringBootApplication
class MeteorologApplication

fun main(args: Array<String>) {
	runApplication<MeteorologApplication>(*args)

}

@Configuration
@EnableScheduling
class Config {}

@Service
class TestDataProvider(
	private val deviceRepository: DeviceRepository,
	private val weatherRepository: WeatherRepository
) {
	private val log: Logger = LoggerFactory.getLogger(this::class.java)

	@Scheduled(initialDelay = 2000, fixedDelay = Long.MAX_VALUE)
	fun putTestData() {
		log.info("Put test data invoked!")
		if (deviceRepository.findAll().isEmpty()) {
			//Put test device
			val device = deviceRepository.save(DeviceEntry(
				id = null,
				serialNumber = "4444",
				name = "Test device",
				ipAddress = "192.168.87.12",
				location = "Sevastopol, SEVGTU, stud gorodok",
				latitude =  44.44F,
				longitude = 42.22F
			))

			(0..24).forEach { tempIndex ->
				weatherRepository.save(
					WeatherEntry(
						id = null,
						time = Instant.now().minusSeconds((10 + tempIndex).toLong()),
						receiveTime = Instant.now().plusSeconds((10 + tempIndex).toLong()),
						device = device,
						temp = Random(10 + tempIndex).nextDouble(),
						pressure = Random(260 + tempIndex).nextDouble(),
						altitude = Random(100 + tempIndex).nextDouble()
						)
				)
			}
			log.warn("Put some test data!")
		} else {
			log.warn("Data already exists!")
		}
	}
}
