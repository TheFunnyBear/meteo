package tema.meteorolog.repositories

import org.springframework.data.jpa.repository.JpaRepository
import tema.meteorolog.entries.WeatherEntry

interface  WeatherRepository : JpaRepository<WeatherEntry, Long> {

}