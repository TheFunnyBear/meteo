package tema.meteorolog.entries

import jakarta.persistence.*
import kotlinx.serialization.Serializable
import org.hibernate.Hibernate
import tema.meteorolog.InstantSerializer
import java.time.Instant

@Entity
@Table(name = "weather_log")
data class WeatherEntry(
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Column(name = "id", nullable = false)
	val id: Long? = null,

	/**
	 * Время выполнения измерения
	 */
	@Column(name = "time")
	@Serializable(with = InstantSerializer::class) val time: Instant,

	/**
	 * Измерительное устройство
	 */
	@OneToOne(cascade = [CascadeType.MERGE])
	val device: DeviceEntry?,

	/**
	 * Температура, в градусах цельсия
	 */
	@Column(name = "temp")
	val temp: Double,

	/**
	 * Атмосферное давление в мм ртутного столба
	 */
	@Column(name = "pressure")
	val pressure: Double,

	/**
	 * Влажность воздуха
	 */
	@Column(name = "humidity")
	val humidity: Double,
)  {
	override fun equals(other: Any?): Boolean {
		if (this === other) return true
		if (other == null || Hibernate.getClass(this) != Hibernate.getClass(other)) return false
		other as WeatherEntry

		return id != null && id == other.id
	}

	override fun hashCode(): Int = javaClass.hashCode()

	@Override
	override fun toString(): String {
		return this::class.simpleName + "(id = $id , time = $time , temp = $temp , pressure = $pressure , humidity = $humidity )"
	}
}