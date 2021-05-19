<template>
    <base-layout page-title="IOT">
        <ion-row> 
            <controls></controls>
        </ion-row>
        <ion-row>
            
        </ion-row>
    </base-layout>
</template>
<script>
import {IonRow} from '@ionic/vue'
import Controls from '../components/Controls.vue'
export default {
    components:{
        IonRow,
        Controls
    }
}
</script>