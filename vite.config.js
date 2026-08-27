import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';

export default defineConfig({
  base: '/JSONFormater/',
  plugins: [react()],
});
