<template>
    <div class="wrapper">
        <ion-grid style=""> 
            <ion-row>
                <ion-col size="5">
                    <button id="btn1" @click="toggleBtn" >
                        {{fanStatus}}
                    </button>
                </ion-col>
                <ion-col size="2">
                    <div class="divider"> </div>
                </ion-col>
                <ion-col size="5">
                    <button :disabled="btnFanState" @click="toggleFan" class="btn">
                        <fan-status :key="fanData" :color="fanColor" :active="fanMode"> </fan-status>    
                    </button>
                </ion-col>
            </ion-row>
        </ion-grid>
    </div>
</template>
<script>
import FanStatus from './FanStatus.vue'
import {IonGrid,IonRow, IonCol} from '@ionic/vue'
export default {
    created(){
        this.getFanStatus()
        setInterval(this.getFanStatus, 5000)
        //setInterval(this.test(), 3000)
    },
    components:{
        FanStatus,
        IonGrid,
        IonRow,
        IonCol
    },
    data(){
        return{
            fanData: 5,
            fanStatus: "A",
            fanMode: "",
            btnState: false,
            btnFanState: true,
            fanColor: ""
        }
    },
    methods:{
        toggleFan(){
            if (this.fanColor == "green" ){
                this.fanColor = "var(--ion-color-danger)"
                this.setFanManuellStatus(0)
            }
            else if (this.fanColor == "var(--ion-color-danger)"){
                this.fanColor = "green"
                this.setFanManuellStatus(1)
            }
        },
        toggleBtn(){
            this.btnFanState = !this.btnFanState
            if (this.fanStatus == "M"){
                this.fanStatus = "A"
                this.setFanManuellStatus(0)
                this.getFanStatus()
            }
            else if (this.fanStatus == "A"){
                this.fanStatus = "M"
            }
        },
        setFanManuellStatus(FanStatus){
            this.$http.get("http://192.168.65.20:3000/fan/setStatus",{ params: { status: FanStatus } }).then(console.log("Fan Status: " + FanStatus))
        },
        getFanStatus(){
            if (this.fanStatus == "A"){
                this.$http.get("http://192.168.65.20:3000/fan/getStatus").then((response) => {
                console.log(response.data)
                this.fanData = response.data.response[0].fan_status
                console.log(this.fanData)
            })
            }
            else if(this.fanStatus == "M"){
                this.$http.get("http://192.168.65.20:3000/fan/getManuellStatus").then((response) => {
                console.log(response.data)
                this.fanData = response.data.response[0].fan_status
                console.log(this.fanData)
                })
            }
            
        },
        test(){
            console.log("test")
            console.log(this.fanMode)
            console.log(this.fanColor)
        }
    },
    watch:{
        fanStatus: function(){
            if(this.fanStatus == "M"){
                this.fanMode = "none"
            }
            else if (this.fanStatus == "A"){
                this.fanMode = ""
            }
        },
        fanData: function(){
            if (this.fanData == 0){
                this.fanColor = "var(--ion-color-danger)"
            }
            else if (this.fanData == 1 || this.fanData == 2){
                this.fanColor = "green"
            }
        }
    }
}
</script>
<style scoped>
.wrapper{
    background-color: grey;
    width: 100%;
    margin: 0;
    padding: 0;
}
.divider{
    left: 40%;
    width: 20%;
    height: 80%;
    border-left: medium solid black;
    position: absolute;
    z-index: 2;
    
}
.center {
  margin: auto;
  width: 100%;
}
.btn{
    width: 100%;
    height: 100%;
    padding: 0;
    margin: 0;
    background-color: transparent;
    border: none;
}
.btn:active{
	background: var(--ion-color-medium-shade);
	transition: all 0.4s ease 0s;
}
button{
    height: 100%;
    width: 100%;
    color: #494949 !important;
    text-transform: uppercase;
    font-size: 25px;
	text-decoration: none;
	background:  var(--ion-color-primary-contrast);
	border: 4px solid var(--ion-color-dark-tint) ;
	display: inline-block;
	transition: all 0.4s ease 0s;
}
button:active{
    color: #ffffff !important;
	background: var(--ion-color-primary);
	border-color: var(--ion-color-primary) !important;
	transition: all 0.4s ease 0s;
}
</style>