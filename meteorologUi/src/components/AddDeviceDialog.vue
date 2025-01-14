<script setup lang="ts">

// Define an interface for the component's data
import {defineComponent, ref} from 'vue';

defineProps<{
  text: String,
  icon: String,
  disabled: Boolean,
}>()

const dialogVisible = ref<boolean>(false);

function clickOnAddButton() {
  console.info("Add button clicked");
  dialogVisible.value = true;
}

function clickOnCancelButton() {
  console.info("Cancel button clicked");
  dialogVisible.value = false;
}

function showDialog() {
  dialogVisible.value = true;
}

</script>

<script lang="ts">
</script>

<template>
  <v-dialog
      v-model="dialogVisible"
      persistent
      @keydown.esc="dialogVisible = false;$emit('close');"
  >
    <template v-slot:activator="{ on, attrs }">
      <v-btn icon tile color="normal" height="48" v-bind="attrs" v-on:click="showDialog" :disabled="disabled">
        <div style="position: relative;" class="mx-2" v-bind="attrs">
          <v-icon color="black" style="position: absolute;" large dense>
            mdi-thermometer
          </v-icon>

          <v-icon color="green" small style="top: 10px; left:10px" >
            {{ icon }}
          </v-icon>
        </div>
      </v-btn>
    </template>

    <v-card
        class="mx-auto"
        elevation="16"
    >
      <v-card-title>
        <span class="text-h5" id="ID_TITLE_ADD_DEVICE">Добавить устройство</span>
      </v-card-title>
      <v-card-text>
        <v-form ref="form" lazy-validation>
          <v-row>
            <v-col cols="12">
              Добавить устройство
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="6"></v-col>
            <v-col cols="6" class="d-flex justify-end mb-6">
              <v-btn class="mr-2" color="success"
                     @click.stop="clickOnAddButton()">
                Добавить
              </v-btn>

              <v-btn class="mr-2" color="info"
                     @click="clickOnCancelButton()">
                Отменить
              </v-btn>
            </v-col>
          </v-row>
        </v-form>
      </v-card-text>
    </v-card>
  </v-dialog>
</template>

<style scoped>
.read-the-docs {
  color: #888;
}
</style>
