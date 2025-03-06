<script setup lang="ts">
import {ref, shallowRef} from 'vue'
import { AgGridVue } from "ag-grid-vue3";
import { AG_GRID_LOCALE_RUS } from "./locale_rus.ts";
import { IWeatherEntry } from  "./weatherEntry.ts"
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
import axios from 'axios'



// Register all Community features
ModuleRegistry.registerModules([AllCommunityModule]);

defineProps<{ msg: string }>()
const count = ref(0)

const gridApi = shallowRef<GridApi<IWeatherEntry> | null>(null);

const columnDefs = ref<ColDef[]>([
  {
    headerName: "Наименование устройства",
    field: "device.name",
    minWidth: 170,
  },
  {
    headerName: "Время на устройстве",
    field: "time"
  },
  {
    headerName: "Время получения",
    field: "receiveTime"
  },
  {
    headerName: "Температура",
    field: "temp"
  },
  {
    headerName: "Давление",
    field: "pressure"
  },
  {
    headerName: "Высота над уровнем моря",
    field: "altitude"
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
const rowData = ref<IWeatherEntry[]>(null);

const onGridReady = (params: GridReadyEvent) => {
  gridApi.value = params.api;

  const updateData = (data) => (rowData.value = data);

  axios.get("http://localhost:8090/api/readings")
  .then((receivedData) => {
        console.info("receivedData:", receivedData.data);
        updateData(receivedData.data);
    }
  );
};

</script>

<template>
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

</template>

<style scoped>
.read-the-docs {
  color: #888;
}
</style>
