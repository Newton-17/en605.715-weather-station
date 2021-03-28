import React, { Component } from "react";
import ReactDOM from "react-dom";
import Head from 'next/head';
import Title from '../components/Title/Title';
import Navbar from '../components/Navbar/Navbar';
import Bargraph from '../components/Bargraph/Bargraph';

class WeatherStation extends Component {
    constructor() {
        super();
        this.state = {
            error: null,
            isLoaded: false,
        };
    }

    async componentDidMount() {
        const options = {
            method: 'GET',
            //mode: 'no-cors',
            headers: {
                'Content-Type': 'text/plain'
            }
        }
        var response = await fetch('http://192.168.1.197', options);
        var text = await response.text();
        console.log(text);

        var lines = text.split("\n");
        var xdata = [];
        var tempYData = [];
        var humidityYData = [];
        for (var line of lines.slice(1)) {
            var splitLine = line.split(',')
            var xDate = new Date(parseInt(splitLine[0], 10) * 1000)
            var xDateFormatted = xDate.getHours() + ":" + xDate.getMinutes() + ":" + xDate.getSeconds();
            xdata.push(xDateFormatted);
            tempYData.push(splitLine[1]);
            humidityYData.push(splitLine[2]);
        }

        console.log();

        this.setState({
            error: null,
            isLoaded: true,
            temperatureData: {
                xdata: xdata,
                ydata: tempYData,
                title: "Temperature vs Time",
                current: tempYData[tempYData.length - 2]
            },
            humidityData: {
                xdata: xdata,
                ydata: humidityYData,
                title: "Humidity vs Time",
                current: humidityYData[humidityYData.length - 2]
            }
        });
        
    }


    render() {
        if (!this.state.isLoaded){
            return <div>Loading Data for Weather Charts</div>
        }
        
        return (
            <div>
                <div>
                    <Title />
                    <Navbar />
                </div>
                <div className="weatherHeader">
                    <h3>Current Temperature: {this.state.temperatureData.current}</h3>
                    <h3>Current Humidity: {this.state.humidityData.current}</h3>
                </div>
                <div>
                    <Bargraph xdata={this.state.temperatureData.xdata} ydata={this.state.temperatureData.ydata} ylabel="Temperature F" title={this.state.temperatureData.title} />
                </div>
                <div>
                    <Bargraph xdata={this.state.humidityData.xdata} ydata={this.state.humidityData.ydata} ylabel="Humidity %" title={this.state.humidityData.title} />
                </div>
            </div>

        );

    }
}

export default WeatherStation;