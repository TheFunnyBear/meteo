package tema.meteorolog

import kotlinx.serialization.ExperimentalSerializationApi
import kotlinx.serialization.KSerializer
import kotlinx.serialization.Serializer
import kotlinx.serialization.descriptors.SerialDescriptor
import kotlinx.serialization.descriptors.buildClassSerialDescriptor
import kotlinx.serialization.encoding.Decoder
import kotlinx.serialization.encoding.Encoder
import java.time.Instant

@OptIn(ExperimentalSerializationApi::class)
@Serializer(forClass = Instant::class)
object InstantSerializer : KSerializer<Instant> {
	override fun deserialize(decoder: Decoder): Instant {
		return Instant.parse(decoder.decodeString())
	}

	override fun serialize(encoder: Encoder, value: Instant) {
		encoder.encodeString(value.toString())
	}

	override val descriptor: SerialDescriptor =
        buildClassSerialDescriptor("artem.time.Instant")
}