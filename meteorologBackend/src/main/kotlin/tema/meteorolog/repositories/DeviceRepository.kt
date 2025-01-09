package tema.meteorolog.repositories

import org.springframework.data.jpa.repository.JpaRepository
import org.springframework.data.jpa.repository.Query
import tema.meteorolog.entries.DeviceEntry

interface  DeviceRepository : JpaRepository<DeviceEntry, Long> {
    @Query(
        value = "SELECT * FROM devices ORDER BY name",
        countQuery = "SELECT count(*) FROM devices ORDER BY name",
        nativeQuery = true
    )
    fun findAllOrdered(): List<DeviceEntry>

    @Query(
        value = "SELECT * FROM devices WHERE name LIKE '%' || :deviceName || '%' ORDER BY name",
        countQuery = "SELECT count(*) FROM devices WHERE name LIKE '%' || :deviceName || '%' ORDER BY name",
        nativeQuery = true
    )
    fun filterByDeviceNameOrdered(deviceName: String): List<DeviceEntry>
}

