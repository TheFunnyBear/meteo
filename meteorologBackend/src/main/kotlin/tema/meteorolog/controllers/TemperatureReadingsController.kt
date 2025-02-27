package tema.meteorolog.controllers

import kotlinx.serialization.Serializable
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import org.springframework.http.HttpStatus
import org.springframework.http.ResponseEntity
import org.springframework.web.bind.annotation.*

@Serializable
data class RequestDto(
    val temperature: Float,
    val pressure: Float,
    val altitude: Float
)

@RestController
@RequestMapping("/api")
class TemperatureReadingsController(

) {

    private val log: Logger = LoggerFactory.getLogger(this::class.java)

    @PostMapping("/test")
    fun testConnection(@RequestBody requestData: RequestDto
    ) : ResponseEntity<Any> {
        log.info("Message received! $requestData")
        return ResponseEntity(HttpStatus.OK)
    }

}