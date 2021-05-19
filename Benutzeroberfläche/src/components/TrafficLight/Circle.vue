<template>
    <div class="circle-outer">
        <div :style="Style" class="circle">
        </div>
    </div>
</template>
<script>
export default {
    props:['color', 'borderColor', 'shadow'],
    computed: {
    Style () {
      return {
        '--circle-color': this.color,
        '--circle-border-color': this.borderColor,
        '--circle-shadow-color' :this.shadow[1],
        '--circle-shadow-size': this.shadow[0]
      }
    }
    }
}
</script>
<style scoped>
div.circle-outer {
    width: 60%;
    padding-bottom: 60%;
    position: relative;
}
div.circle {
    box-shadow: 0px 0px var(--circle-shadow-size) var(--circle-shadow-color);
    border-style: solid;
    border-width: 2%;
    border-color: var(--circle-border-color);
    position: absolute;
    top: 0;
    right: 0;
    bottom: 0;
    left: 0;
    background-color: var(--circle-color);
    border-radius: 50%;
}
</style>