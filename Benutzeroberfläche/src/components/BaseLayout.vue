<template>
    <ion-app>
        <ion-header>
            <ion-toolbar color="primary">
                <ion-title>{{pageTitle}}</ion-title>
            </ion-toolbar>
        </ion-header>

        <ion-content class="ion-padding">
            <slot/>
        </ion-content>
    </ion-app>
</template>
<script>
import {IonApp, IonHeader, IonToolbar, IonTitle, IonContent} from '@ionic/vue'
export default {
    components:{
        IonApp,
        IonHeader,
        IonToolbar,
        IonTitle,
        IonContent
    },
    props:['pageTitle']
}
</script>