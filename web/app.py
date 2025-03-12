from flask import Flask, render_template, jsonify, request
import subprocess
import json
import os
from threading import Thread
import numpy as np

app = Flask(__name__)

# Proje kök dizinini belirle
PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
NEURAL_SIM_PATH = os.path.join(PROJECT_ROOT, 'build', 'neural_sim')
OUTPUT_DIR = os.path.join(PROJECT_ROOT, 'output')

simulation_running = False
current_progress = 0

def read_network_state(filename):
    data = {}
    try:
        with open(filename, 'r') as f:
            lines = f.readlines()
            data['time'] = float(lines[0].split(': ')[1])
            data['pyramidal'] = float(lines[1].split(': ')[1].split(' Hz')[0])
            data['inhibitory'] = float(lines[2].split(': ')[1].split(' Hz')[0])
    except:
        pass
    return data

def run_simulation(config):
    global simulation_running, current_progress
    
    # Çıktı dizinini oluştur
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    
    # Geçici config dosyasını proje kök dizinine kaydet
    config_path = os.path.join(PROJECT_ROOT, 'temp_config.ini')
    with open(config_path, 'w') as f:
        for key, value in config.items():
            f.write(f"{key} = {value}\n")
    
    # Simülasyonu çalıştır
    simulation_running = True
    try:
        process = subprocess.Popen(
            [NEURAL_SIM_PATH, '-c', config_path, '-o', OUTPUT_DIR],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            cwd=PROJECT_ROOT  # Çalışma dizinini proje kökü yap
        )
        
        while simulation_running:
            line = process.stdout.readline()
            if not line:
                break
            if b'progress' in line:
                current_progress = float(line.split(b'%')[0].split(b': ')[1])
    finally:
        simulation_running = False
        if os.path.exists(config_path):
            os.remove(config_path)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/start_simulation', methods=['POST'])
def start_simulation():
    if not os.path.exists(NEURAL_SIM_PATH):
        return jsonify({
            'status': 'error',
            'message': 'Neural simulation executable not found. Please build the project first.'
        }), 400
    
    config = request.json
    Thread(target=run_simulation, args=(config,)).start()
    return jsonify({'status': 'started'})

@app.route('/simulation_status')
def simulation_status():
    return jsonify({
        'running': simulation_running,
        'progress': current_progress
    })

@app.route('/get_results')
def get_results():
    states = []
    if os.path.exists(OUTPUT_DIR):
        for file in sorted(os.listdir(OUTPUT_DIR)):
            if file.startswith('network_state_'):
                state = read_network_state(os.path.join(OUTPUT_DIR, file))
                if state:
                    states.append(state)
    
    return jsonify({'states': states})

if __name__ == '__main__':
    print(f"Project root: {PROJECT_ROOT}")
    print(f"Neural sim path: {NEURAL_SIM_PATH}")
    print(f"Output directory: {OUTPUT_DIR}")
    app.run(debug=True) 