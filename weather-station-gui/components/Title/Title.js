import react, { Component } from 'react';
import styles from "./Title.module.css";

export default class Navbar extends Component {
    render() {
        return (
            <div className={styles.titleHeader}>
                <h1>Embedded Software Engineering Portal</h1>
            </div>
        )
    }
}