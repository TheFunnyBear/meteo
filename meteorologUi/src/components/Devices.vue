<script setup lang="ts">
import {ref, shallowRef} from 'vue'
import { AgGridVue } from "ag-grid-vue3";
import { AG_GRID_LOCALE_RUS } from "./locale_rus.ts";
import { IDeviceEntry } from  "./deviceEntry.ts"
import {
  AllCommunityModule,
  ColDef,
  ColGroupDef,
  GridApi,
  GridOptions,
  GridReadyEvent,
  IServerSideDatasource,
  IServerSideGetRowsRequest,
  ModuleRegistry,
  RowModelType,
  ValidationModule,
  createGrid,
} from "ag-grid-community";
import AddDeviceDialog from "./AddDeviceDialog.vue";
import EditDeviceDialog from "./EditDeviceDialog.vue";
import DeleteDeviceDialog from "./DeleteDeviceDialog.vue";
import axios from 'axios'


// Register all Community features
ModuleRegistry.registerModules([AllCommunityModule]);

defineProps<{ msg: string }>()
const count = ref(0)

const gridApi = shallowRef<GridApi<IOlympicData> | null>(null);

const columnDefs = ref<ColDef[]>([
  {
    headerName: "Наименование устройства",
    field: "name",
    minWidth: 170,
  },
    {
      headerName: "Серийный номер устройства",
      field: "serialNumber"
    },
  {
    headerName: "Интернет адрес устройства",
    field: "ipAddress"
  },
  {
    headerName: "Расположение устройства",
    field: "location"
  },
  {
    headerName: "Широта",
    field: "longitude"
  },
  {
    headerName: "Долгота",
    field: "latitude"
  },
]);


const localeText = ref<{[key: string]: string;}>(AG_GRID_LOCALE_RUS);

const defaultColDef = ref<ColDef>({
  editable: true,
  filter: true,
  flex: 1,
  minWidth: 100,
});
const rowSelection = ref<RowSelectionOptions | "single" | "multiple">({
  mode: "multiRow",
  groupSelects: "descendants",
});
const rowData = ref<IDeviceEntry[]>(null);

const onGridReady = (params: GridReadyEvent) => {
  gridApi.value = params.api;

  const updateData = (data) => (rowData.value = data);

  console.info("Grid ready");
  axios.get("http://localhost:8090/api/devices")
  .then((receivedData) => {
        console.info("receivedData:", receivedData.data);
        updateData(receivedData.data);
    }
  );
};

</script>

<template>
  <v-row>
    <v-col cols="12">
      <v-toolbar>

        <v-toolbar-title id="#ID_TITLE_DEVICES">Измерительные устройства
        </v-toolbar-title>

        <v-divider class="mx-2" inset vertical/>
        <AddDeviceDialog
            id="ID_ADD_DEVICE_DIALOG_BUTTON"
            icon="mdi-plus-circle"
            text="Добавить устройство"
            :disabled=false
        >
        </AddDeviceDialog>

        <EditDeviceDialog
          id="ID_EDIT_DEVICE_DIALOG_BUTTON"
          icon="mdi-pencil"
          text="Редактировать устройство"
          :disabled=false
        >
        </EditDeviceDialog>

        <DeleteDeviceDialog
            id="ID_DELETE_DEVICE_DIALOG_BUTTON"
            icon="mdi-delete"
            text="Удалить устройство"
            :disabled=false
        >
        </DeleteDeviceDialog>

        <v-divider class="mx-2" inset vertical/>
        <v-spacer/>
        <v-spacer/>
      </v-toolbar>
    </v-col>
    <v-col cols="12">
        <ag-grid-vue
          style="height: 1200px; width: 1200px;"
          @grid-ready="onGridReady"
          :columnDefs="columnDefs"
          :defaultColDef="defaultColDef"
          :rowSelection="rowSelection"
          :pagination="true"
          :rowData="rowData"
          :localeText="localeText"
      >
      </ag-grid-vue>
    </v-col>
  </v-row>
</template>

<style scoped>
.read-the-docs {
  color: #888;
}
</style>
