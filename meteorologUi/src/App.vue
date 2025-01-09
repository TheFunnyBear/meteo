<script setup lang="ts">
import HelloWorld from './components/HelloWorld.vue'
import {shallowRef} from "vue";

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
          </v-tabs>

          <v-tabs-window v-model="tab"  class="ma-5">
            <v-tabs-window-item value="DEVICES">
              <HelloWorld msg="Vite + Vue"/>
            </v-tabs-window-item>

            <v-tabs-window-item value="WHETHER">
              <HelloWorld msg="?"></HelloWorld>
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
