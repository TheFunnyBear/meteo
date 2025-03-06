<script setup lang="ts">
import TemperatureReadings from './components/TemperatureReadings.vue'
import TemperatureChart from './components/TemperatureChart.vue'
import {shallowRef} from "vue";
import Devices from "./components/Devices.vue";

const tab = shallowRef('DEVICES')


</script>

<template>
  <v-app class="d-flex">
    <v-row>
      <v-col cols="12" class="align-center text-center">
        <div class="flex-row ">
          <v-tabs v-model="tab" class="text-center w-100">
            <v-tab value="DEVICES">Устройства</v-tab>
            <v-tab value="WHETHER">Измерения</v-tab>
            <v-tab value="TEMPERATURE_CHART">График температур</v-tab>
          </v-tabs>

          <v-tabs-window v-model="tab"  class="ma-5">
            <v-tabs-window-item value="DEVICES">
              <Devices msg=""/>
            </v-tabs-window-item>

            <v-tabs-window-item value="WHETHER">
              <TemperatureReadings msg="?"></TemperatureReadings>
            </v-tabs-window-item>

            <v-tabs-window-item value="TEMPERATURE_CHART">
              <TemperatureChart></TemperatureChart>
            </v-tabs-window-item>
          </v-tabs-window>
        </div>
      </v-col>
    </v-row>

    <v-footer ma-5>
      <div class="px-4 py-2 text-center w-100">
        {{ new Date().getFullYear() }} — <strong>Метеоролог</strong>
      </div>
    </v-footer>
  </v-app>
</template>

<style scoped>

</style>
