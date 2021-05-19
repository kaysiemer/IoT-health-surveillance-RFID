<template>
    <div class="wrapper">
        <window :key="status" :open="windowOpen" :close="windowClose"> </window>     
    </div>
</template>
<script>
import Window from './window.vue'
export default {
    props:['status'],
    components:{
        Window
    },
    data(){
        return{
            windowOpen: "none",
            windowClose: "none"
        }
    },
    watch:{
        status: function(){
            if (this.status == 1  ){
                this.windowOpen = "none"
                this.windowClose = ""
                console.log("open")
            }
            else if (this.status == 0){
                this.windowOpen = ""
                this.windowClose = "none"
                console.log("closed")
            }
        }
    },
    methods: {
        test(){
            console.log(this.status)
            console.log(this.windowOpen)
            console.log(this.windowClose)
        }
    }
}
</script>
<style scoped>
.wrapper{
    background-color: grey;
    width: 100%;
    padding: 5px;
}
</style>