import react, { Component } from 'react';
import styles from "./Bargraph.module.css";
import { Line } from 'react-chartjs-2';



/*
* Expected Props
* data
* xdata
* ydata
* xlabel
* ylabel
* title
*/
export default class Bargraph extends Component {
    constructor(props) {
        super(props);
    }
    render() {
        const data = {
            labels: this.props.xdata,
            datasets: [
              {
                label: this.props.ylabel,
                data: this.props.ydata,
                //data: [12, 19, 3, 5, 2, 3],
                fill: false,
                backgroundColor: 'rgb(255, 99, 132)',
                borderColor: 'rgba(255, 99, 132, 0.2)',
              },
            ],
          }
          
          const options = {
            scales: {
              yAxes: [
                {
                  ticks: {
                    beginAtZero: true,
                  },
                },
              ],
            },
            title: {
              display: true,
              fontSize: 16,
              text: this.props.title
            }
          }
        return (
            <div className={styles.linegraph}>
                 <Line data={data} options={options} />
            </div>
        )
    }
}