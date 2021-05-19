<template>
    <div class="wrapper">
        <button id="btn1" >
            {{peopleInRoom}}{{people}}
        </button>
    </div>
</template>
<script>
 
export default {
    props: ['peopleInRoom'],
    data(){
        return{
            people: " Personen"
        }
    }
}
</script>
<style>
.wrapper{
    background-color: grey;
    width: 100%;
    padding: 5px;
}
#ionicon{
    height: 20px;
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