import react, { Component } from 'react';
import styles from "./Navbar.module.css";

export default class Navbar extends Component {
    render() {
        return (
            <div className={styles.topnav}>
                <a href="/">Home</a>
                <a href="/weather-station">Weather Station</a>
            </div>
        )
    }
}