<script setup lang="ts">
import {ref, shallowRef} from 'vue'
import { AgGridVue } from "ag-grid-vue3";
import { AG_GRID_LOCALE_RUS } from "./locale_rus.ts";
import { IOlympicData } from  "./olimpic.ts"
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



// Register all Community features
ModuleRegistry.registerModules([AllCommunityModule]);

defineProps<{ msg: string }>()
const count = ref(0)

const gridApi = shallowRef<GridApi<IOlympicData> | null>(null);

const columnDefs = ref<ColDef[]>([
  {
    headerName: "Наименование устройства",
    field: "athlete",
    minWidth: 170,
  },
  {
    headerName: "Интернет адрес устройства",
    field: "age"
  },
  {
    headerName: "Расположение устройства",
    field: "country"
  },
  {
    headerName: "Широта",
    field: "year"
  },
  {
    headerName: "Долгота",
    field: "date"
  },
  { field: "sport" },
  { field: "gold" },
  { field: "silver" },
  { field: "bronze" },
  { field: "total" },
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
const rowData = ref<IOlympicData[]>(null);

const onGridReady = (params: GridReadyEvent) => {
  gridApi.value = params.api;

  const updateData = (data) => (rowData.value = data);

  fetch("https://www.ag-grid.com/example-assets/olympic-winners.json")
      .then((resp) => resp.json())
      .then((data) => updateData(data));
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
    <!--
      <h1>{{ msg }}</h1>


      <div class="card">
        <button type="button" @click="count++">count is {{ count }}</button>
        <p>
          Edit
          <code>components/HelloWorld.vue</code> to test HMR
        </p>
      </div>

      <p>
        Check out
        <a href="https://vuejs.org/guide/quick-start.html#local" target="_blank"
          >create-vue</a
        >, the official Vue + Vite starter
      </p>
      <p>
        Learn more about IDE Support for Vue in the
        <a
          href="https://vuejs.org/guide/scaling-up/tooling.html#ide-support"
          target="_blank"
          >Vue Docs Scaling up Guide</a
        >.
      </p>
      <p class="read-the-docs">Click on the Vite and Vue logos to learn more</p>
    -->




</template>

<style scoped>
.read-the-docs {
  color: #888;
}
</style>
