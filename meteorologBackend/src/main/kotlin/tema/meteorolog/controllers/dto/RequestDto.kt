package tema.meteorolog.controllers.dto

import kotlinx.serialization.Serializable

@Serializable
data class RequestDto(
    val serialNumber: String,
    val time: String,
    val temperature: Float,
    val pressure: Float,
    val altitude: Float
)