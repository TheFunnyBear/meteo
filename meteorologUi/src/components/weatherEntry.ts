import { IDeviceEntry } from  "./deviceEntry.ts"

export interface IWeatherEntry {
    val id: number,
    val time: string,
    val receiveTime: string,
    val device: IDeviceEntry,
    val temp: number,
    val pressure: number,
    val altitude: number
}