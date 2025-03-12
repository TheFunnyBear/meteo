import { IDeviceEntry } from  "./deviceEntry.ts"

export interface IWeatherEntry {
    id: number,
    time: string,
    receiveTime: string,
    device: IDeviceEntry,
    temp: number,
    pressure: number,
    altitude: number,
}