<template>
    <ion-grid>
        <ion-row>
            <circle-traffic-light :key="color.top.background" :color="color.top.background" :borderColor="color.top.border" :shadow="color.top.shadow"></circle-traffic-light>
        </ion-row>
        <ion-row>
            <circle-traffic-light :key="color.middle1.background" :color="color.middle1.background" :borderColor="color.middle1.border" :shadow="color.middle1.shadow"></circle-traffic-light>
        </ion-row>
        <ion-row>
            <circle-traffic-light :key="color.middle2.background" :color="color.middle2.background" :borderColor="color.middle2.border" :shadow="color.middle2.shadow"></circle-traffic-light>
        </ion-row>
        <ion-row>
            <circle-traffic-light :key="color.bottom.background" :color="color.bottom.background" :borderColor="color.bottom.border" :shadow="color.bottom.shadow"></circle-traffic-light>
        </ion-row>
    </ion-grid>
</template>
<script>
import {IonGrid, IonRow} from '@ionic/vue'
import CircleTrafficLight from './Circle.vue'

export default {
    created(){
    },
    props: ['status'],
    components:{
        IonGrid,
        IonRow,
        CircleTrafficLight
    },
    data(){
        return{
            color: {
                top:{background:"",border:"", shadow: ["0px", "black"]},
                middle1:{background:"",border:"", shadow: ["0px", "black"]},
                middle2:{background:"",border:"", shadow: ["0px", "black"]},
                bottom:{background:"",border:"", shadow: ["0px", "black"]}
            }
        }
    },
    methods:{
        test(){
            console.log("status")
            console.log(this.status)
        }
    },
    watch:{
        status: function(){
            if(this.status == 4){
                this.color = {
                    top:{background:"#FF0000",border:"#FF0000", shadow: ["20px", "#FF0000" ]}, 
                    middle1:{background:"#FFB6C1", border:"#FF34B3", shadow: ["3px","grey"]}, 
                    middle2:{background:"#FFFACD",border:"#FFFF00", shadow: ["3px","grey"]}, 
                    bottom:{background:"#B4EEB4",border:"#228B22", shadow: ["3px","grey"]}}
            }
            else if (this.status == 3){
                this.color = {
                    top:{background:"#FFA07A",border:"#FF0000", shadow: ["3px","grey"]}, 
                    middle1:{background:"#FF34B3",border:"#FF34B3", shadow: ["20px","#FF34B3"]}, 
                    middle2:{background:"#FFFACD",border:"#FFFF00", shadow: ["3px","grey"]}, 
                    bottom:{background:"#B4EEB4",border:"#228B22", shadow: ["3px","grey"]}}
            }
            else if (this.status == 2){
                this.color = {
                    top:{background:"#FFA07A",border:"#FF0000", shadow: ["3px","grey"]}, 
                    middle1:{background:"#FFB6C1",border:"#FF34B3", shadow: ["3px","grey"]}, 
                    middle2:{background:"#FFFF00",border:"#FFFF00", shadow: ["20px","#FFFF00"]}, 
                    bottom:{background:"#B4EEB4",border:"#228B22", shadow: ["3px","#grey"]}}
            }
            else if (this.status == 1){
                this.color = {
                    top:{background:"#FFA07A",border:"#FF0000", shadow: ["3px","grey"]}, 
                    middle1:{background:"#FFB6C1",border:"#FF34B3", shadow: ["3px","grey"]}, 
                    middle2:{background:"#FFFACD",border:"#FFFF00", shadow: ["3px","grey"]}, 
                    bottom:{background:"#228B22",border:"#228B22", shadow: ["20px","#228B22"]}}   
            }
            else {
                this.color = {top:{background:"#FF0000",border:"#FF0000", shadow: ["20px", "#FF0000"]}, 
                middle1:{background:"#FF34B3",border:"#FF34B3", shadow: ["20px","#FF34B3"]}, 
                middle2:{background:"#FFFF00",border:"#FFFF00", shadow: ["20px","#FFFF00"]}, 
                bottom:{background:"#228B22",border:"#228B22", shadow: ["20px","#228B22"]}}
            }
        }
    }
}
</script>
<style scoped>

</style>