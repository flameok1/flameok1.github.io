var app = Vue.createApp({
    data:function() {
        return {
           message:'Hello World',
           value1:'Hello World',
           value2:'Hello Vue',
           bshow: true,
           nshow: 3,
           testinput: '',
           fruit:[],
           grade:'',
           count:0,
           name:'',
           age:'',
           top:50,
           left:50,

           list:[
                {name:'yvonne',age:21}
            ],


           mapname:{
                one:"yvonne",
                two:"emily",
                three:"karina",
            }
        };
    },
    methods:{
        onClickA(value){
            this.count+=value;
        },

        add()
        {
            this.list.push(
                {
                    name:this.name,
                    age:this.age,
                }
            );
        },

        remove(index)
        {
            this.list.splice(index,1);
        },

        moveY(value){
            this.top+=value;
        },

        moveX(value){
            this.left+=value;
        }

    },
    computed:{
        fullName(){
            return this.lastName + this.firstName;
        },
        remaining:{
            get()
            {
                return this.haveMoney - this.used;
            }
            ,
            set(value)
            {
                return value;
            },
            
        }
    },
});
app.mount('#app');