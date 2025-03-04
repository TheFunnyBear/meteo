package tema.meteorolog.entries

import javax.persistence.*


@Entity
@Table(name = "devices")
data class DeviceEntry(
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Column(name = "id", nullable = false)
	val id: Long? = null,

	/**
	 * Серийный номер устройства
	 */
	@Column(name = "serial_number")
	val serialNumber: String,

	/**
	 * Наименование устройства
	 */
	@Column(name = "name")
	val name: String,

	/**
	 * Интернет адрес устройства
	 */
	@Column(name = "ip_address")
	val ipAddress: String,

	/**
	 * Расположение устройства
	 */
	@Column(name = "location")
	val location: String,

	/**
	 * Широта
	 */
	@Column(name = "latitude")
	val latitude: Float,

	/**
	 * Долгота
	 */
	@Column(name = "longitude")
	val longitude: Float,
) {

}