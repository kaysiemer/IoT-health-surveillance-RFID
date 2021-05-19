<template>
    <v-flex>
        <GChart
        type="LineChart"
        :data="chartData"
        :options="chartOptions"
        /> 
    </v-flex>
</template>
<script>
import {GChart} from 'vue-google-charts'
export default {
    components:{
        GChart
    },
    created(){
        this.getChartData()
        //(setInterval(this.getChartData, 5000)
    },
    data(){
        return {
            chartData: [["Timestamp","Co2", "Luftfeuchtigkeit", "Luftdruck", "Temperatur"],[new Date(), null, null, null, null]],
            chartOptions: {
                interpolateNulls: true,
                selectionMode: 'multiple',
                explorer: {
                    maxZoomIn: .05,
                    axis: 'horizontal',
                    actions: ['dragToZoom', 'rightClickToReset']
                },
                legend: {
                    position: 'top',
                    alignment: 'start',
                    textStyle:{color: '#ffffff'}
                },
                lineWidth: 3,
                height: 400,
                animation: {
                    startup: true,
                    duration: 500,
                    easing: 'in'
                },
                backgroundColor: 'grey',
                chartArea: {
                    backgroundColor: "#ffffff",
                    right: '15%',
                    left: '15%',
                    top: '9%',
                    height: '100%',
                    bottom: '9%'
                },
                colors:['orange', '#3880ff', '#FF4500', '#76EE00'],
                tooltip:{
                    trigger: 'hover'
                },
                hAxis: {
                    gridlines: {
                        count: -1,
                        units: {
                            years: {format: ['yyyy']},
                            months: {format:['dd.MM']},
                            days: {format:['dd.MM']},
                            hours: {format:['']}
                        }
                    },
                    minorGridlines:{
                        count: 0,
                        units: {
                            years: {format:['yyyy']},
                            months: {format:['MMMM']},
                            days: {format:['dd.MMM']}
                        }
                    }
                },
                vAxis:{
                    format: 'short',
                    minorGridlines: {
                        count: 0
                    },
                    minValue: 0,
                    viewWindowMode: 'pretty'
                },
                vAxes: {
                    0: {
                        title: 'PPM / hPa'
                    },
                    1:{
                        title: '°C / %'
                    },
                    // Adds labels to each axis; they don't have to match the axis names.
                    y: {
                        Temps: {label: 'Temps (Celsius)'},
                        Daylight: {label: 'Daylight'}
                    }
                },
                series:{
                    0: {targetAxisIndex: 0},
                    1: {targetAxisIndex: 1},
                    2: {targetAxisIndex: 0},
                    3: {targetAxisIndex: 1}
                }
            }
        }
    },
    methods:{
        getChartData(){
            console.log(this.chartData[0])
            this.$http.get("http://192.168.65.20:3000/diagramm/data").then((response) => {
                //console.log(response.data.response[0].timestamp)
                for (var i = 0; i<=Object.keys(response.data.response).length-1; i++){
                    this.chartData.push([new Date(response.data.response[i].timestamp), response.data.response[i].co2, response.data.response[i].humidity, response.data.response[i].airpressur, response.data.response[i].temperature])
                }
                console.log("ChartData")
                console.log(this.chartData)
            })
        },
    }
}
</script>
<style> 

</style>
