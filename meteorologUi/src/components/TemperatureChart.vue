<script setup lang="ts">
import { ref, onMounted } from 'vue';
import axios from 'axios';
import VChart from 'vue-echarts';
import * as echarts from 'echarts';

// Импортируем нужные компоненты и рендереры
import { LineChart} from 'echarts/charts';
import { CanvasRenderer } from 'echarts/renderers';

// Регистрируем компоненты и рендереры
echarts.use([LineChart,   CanvasRenderer]);

interface IWeatherEntry {
  time: string;
  temp: number;
}

const tempChartOption = ref<echarts.EChartsOption | null>(null);
const pressureChartOption = ref<echarts.EChartsOption | null>(null);
const weatherData = ref<IWeatherEntry[]>([]);
const isDataLoaded = ref(false);

const updateChart = (data: IWeatherEntry[]) => {
  if (!data || data.length === 0) {
    console.error('Данные отсутствуют или пусты');
    return;
  }

  const times = data.map(item => item.time.slice(11, 19));  // Читаем только время из строки
  const temps = data.map(item => item.temp);  // Массив температур
  const pressures = data.map(item => item.pressure);

  tempChartOption.value = {
    tooltip: {
      trigger: 'axis',
      axisPointer: {
        type: 'cross',
      },
    },
    xAxis: {
      type: 'category',
      data: times,
    },
    yAxis: {
      type: 'value',
      name: 'Температура',
      min: 0,
      max: 1,
    },
    series: [
      {
        name: 'Температура',
        type: 'line',
        data: temps,
        smooth: true,
        itemStyle: {
          color: '#fc3912',
        },
      },
    ],
  };

pressureChartOption.value = {
    tooltip: {
      trigger: 'axis',
      axisPointer: {
        type: 'cross',
      },
    },
    xAxis: {
      type: 'category',
      data: times,
    },
    yAxis: {
      type: 'value',
      name: 'Давление',
      min: 0,
      max: 1,
    },
    series: [
      {
        name: 'Давление',
        type: 'line',
        data: pressures,
        smooth: true,
        itemStyle: {
          color: '#f39c12',
        },
      },
    ],
  };
};

onMounted(() => {
  axios.get('http://localhost:8090/api/readings')
    .then((response) => {
      weatherData.value = response.data;
      if (weatherData.value.length > 0) {
        updateChart(weatherData.value);
        isDataLoaded.value = true;
      } else {
        isDataLoaded.value = false;
      }
    })
    .catch((error) => {
      console.error('Ошибка при получении данных:', error);
      isDataLoaded.value = false;
    });
});
</script>

<template>
  <div>
    <div v-if="isDataLoaded && tempChartOption">
        График температуры:
        <v-chart  :option="tempChartOption" style="width: 100%; height: 400px;"></v-chart>

        График давления:
        <v-chart  :option="pressureChartOption" style="width: 100%; height: 400px;"></v-chart>
    </div>
    <div v-else>Загрузка данных...</div>
  </div>
</template>

<style scoped>
/* Стили для компонента */
</style>
