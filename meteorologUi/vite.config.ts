import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

// https://vite.dev/config/
export default defineConfig({
    display: 'standalone',
    start_url: '/',
    base: './',
    plugins: [vue()],
})
