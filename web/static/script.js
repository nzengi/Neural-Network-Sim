document.getElementById('simulation-form').addEventListener('submit', async (e) => {
    e.preventDefault();
    
    // Disable form while simulation runs
    const form = e.target;
    const submitButton = form.querySelector('button[type="submit"]');
    submitButton.disabled = true;
    submitButton.textContent = 'Running...';
    
    const config = {
        num_pyramidal: parseInt(document.getElementById('num-pyramidal').value),
        num_inhibitory: parseInt(document.getElementById('num-inhibitory').value),
        connection_rate: parseFloat(document.getElementById('connection-rate').value),
        simulation_time: parseFloat(document.getElementById('sim-time').value),
        dt: 0.001,  // Add simulation timestep
        output_interval: 0.1  // Add output interval
    };
    
    // Start simulation
    document.getElementById('progress-container').style.display = 'block';
    const response = await fetch('/start_simulation', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(config)
    });

    const result = await response.json();
    if (result.status === 'error') {
        alert(result.message);
        submitButton.disabled = false;
        submitButton.textContent = 'Start Simulation';
        return;
    }
    
    // Poll simulation status
    const statusInterval = setInterval(async () => {
        const response = await fetch('/simulation_status');
        const status = await response.json();
        
        document.getElementById('progress').style.width = `${status.progress}%`;
        document.getElementById('progress-text').textContent = `${status.progress.toFixed(1)}%`;
        
        if (!status.running) {
            clearInterval(statusInterval);
            submitButton.disabled = false;
            submitButton.textContent = 'Start Simulation';
            updateResults();
        }
    }, 1000);
});

async function updateResults() {
    const response = await fetch('/get_results');
    const data = await response.json();
    
    if (data.states.length === 0) {
        console.error('No simulation results found');
        return;
    }
    
    const times = data.states.map(s => s.time);
    const pyramidal = data.states.map(s => s.pyramidal);
    const inhibitory = data.states.map(s => s.inhibitory);
    
    Plotly.newPlot('activity-plot', [
        {
            x: times,
            y: pyramidal,
            name: 'Pyramidal Neurons',
            type: 'scatter',
            mode: 'lines',
            line: {color: '#1f77b4'}
        },
        {
            x: times,
            y: inhibitory,
            name: 'Inhibitory Neurons',
            type: 'scatter',
            mode: 'lines',
            line: {color: '#ff7f0e'}
        }
    ], {
        title: 'Neural Activity Over Time',
        xaxis: {
            title: 'Time (ms)',
            gridcolor: '#eee'
        },
        yaxis: {
            title: 'Firing Rate (Hz)',
            gridcolor: '#eee'
        },
        paper_bgcolor: 'white',
        plot_bgcolor: 'white',
        showlegend: true,
        legend: {
            x: 1,
            xanchor: 'right',
            y: 1
        }
    });
} 