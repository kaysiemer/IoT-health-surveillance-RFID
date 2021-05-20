<template>

    <div class="card">
        <ion-grid> 
            <ion-row>
                <ion-col size="5" >
                    <traffic-light :status="riskPotential" style="position: relative; top:0; bottom:0; left:25%;"></traffic-light>
                </ion-col>
                <ion-col size="2">
                    <div class="divider" > </div>
                </ion-col>
                <ion-col size="5">
                    <fan-tile> </fan-tile>
                    <window-tile style="margin-top: 5%" :status="windowStatus"> </window-tile>
                    <people-in-room :peopleInRoom="peopleInRoom"> </people-in-room>
                </ion-col>
            </ion-row>
            <ion-row>
                <ion-col size="12">
                    <line-diagramm-tile :chartData="chartData"></line-diagramm-tile> 
                </ion-col>
            </ion-row>
        </ion-grid>
    </div>
</template>
<script>
import {IonGrid,IonRow, IonCol} from '@ionic/vue'
import TrafficLight from './TrafficLight/TrafficLight.vue'
import FanTile from './Fan/FanTile.vue'
import WindowTile from './Window/windowTile.vue'
import PeopleInRoom from './PeopleInRoom/peopleInRoom.vue'
import LineDiagrammTile from './LineDiagramm/LineDiagramm.vue'
export default {
    created(){
        this.refreshData()
        setInterval(this.refreshData, 2000)
    },
    components:{
        IonRow,
        IonCol,
        IonGrid,
        TrafficLight,
        FanTile,
        WindowTile,
        PeopleInRoom,
        LineDiagrammTile
    },
    data(){
        return{
            riskPotential: 0,
            windowStatus: 2,
            peopleInRoom: 0
        }
    },
    methods: {
        getRiskPotentialStatus(){
            this.$http.get("http://be450b9bbd87.sn.mynetname.net:3000/riskPotential/status").then((response) => {
                //console.log(response.data)
                this.riskPotential = response.data.response[0].risk_potential
                //console.log(this.riskPotential)
            })
        },
        getPeopleInRoom(){
            this.$http.get("http://be450b9bbd87.sn.mynetname.net:3000/peopleInRoom/status").then((response) => {
                //console.log(response.data)
                this.peopleInRoom = response.data.response[0].people
                //console.log(this.riskPotential)
            })
        },
        getWindowStatus(){
            this.$http.get("http://be450b9bbd87.sn.mynetname.net:3000/window/status").then((response) => {
                //console.log(response.data)
                this.windowStatus = response.data.response[0].window_status
                //console.log(this.windowStatus)
            })
        },
        test(){
            console.log("Risikostufe")
            console.log(this.riskPotential)
        },
        refreshData(){
            this.getWindowStatus()
            console.log(this.windowStatus)
            this.getRiskPotentialStatus()
            this.getPeopleInRoom()
            console.log("PeopleInRoom:")
            console.log(this.peopleInRoom)
            console.log("Refreshed")
        }

    },
}
</script>
<style scoped>
.card{
    width: 100%;
    background-color: var(--ion-color-medium);
    box-shadow: 0px 0px 1px var(--ion-color-medium);
}
.divider{
  content:''; /* 2. Create the pseudo-element */
  height: 100%;
  position: relative; top:0; bottom:0; left:50%;  /* 3. Position the pseudo-element */
  width:5px; background:black; /* 5. Style the pseudo-element */
}
.wrapper{
    background-color: grey;
    width: 100%;
    padding: 5px;
}
</style>