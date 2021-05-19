import { createRouter, createWebHistory } from '@ionic/vue-router';
import Home from '../pages/home.vue'
const routes = [
  {
    path: '/',
    redirect: '/home'
  },
  {
    path: '/home',
    component: Home
  }
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

export default router
