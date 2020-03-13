<template>
  <div id="app">
    <md-card md-with-hover md-fullscreen=false>
      <md-ripple>
        <md-card-header>
          <div class="md-title">Gate Client</div>
          <div class="md-subhead">Gate Client bade by @calebebrim</div>
        </md-card-header>

        <md-card-content>
          Pressione o botão para abrir o portão preencha o endereço do portão clique em 'Ativar Controle'
          <md-field>
            <md-input v-model="server"></md-input>
            <label>Endereço</label>
          </md-field>

        </md-card-content>

        <md-card-actions>
          <md-button @click="open">Ativar Controle</md-button>
        </md-card-actions>
      </md-ripple>
    </md-card>
  </div>
</template>

<script>
const axios = require('axios');
import Vue from 'vue'
import VueMaterial from 'vue-material'
import 'vue-material/dist/vue-material.min.css'
import 'vue-material/dist/theme/default.css'
Vue.use(VueMaterial)

export default {
  name: 'App',
  components: {
  },
  data: () => ({
    server: '192.168.0.100'
  }),
  methods: {
    open() {
      console.log(this.server)
      axios.get(`http://${this.server}/opengate=1`).then(response => {
        console.log(response)
      }, error=> {
        console.log(error)
      } )
    }
  }

}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
