import Head from 'next/head';
import Title from '../components/Title/Title'
import Navbar from '../components/Navbar/Navbar'
import WeatherStation from './weather-station'

export default function Home() {
  return (
    <div>
      <Title />
      <Navbar />
    </div>
  )
}