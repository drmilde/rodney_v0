from fastapi import FastAPI
from robotsynth import RobotSynth
import uvicorn
from fastapi.responses import HTMLResponse
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()
robot = RobotSynth()


# WICHTIG: CORS aktivieren, damit der Browser die Requests zulässt
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/sound/{action}")
async def trigger_sound(action: str):
    actions = {
        "blink": robot.play_blink,
        "horn": robot.play_horn,
        "stop": robot.play_stop,
        "happy": robot.play_happy,
        "negative": robot.play_negative,
        "attention": robot.play_attention,
        "dreamy": robot.dreamy,
        "error": robot.play_error
    }
    
    if action in actions:
        actions[action]()
        return {"status": "playing", "action": action}
    return {"status": "error", "message": "Action not found"}

@app.get("/drive/{speed}")
async def set_drive_speed(speed: float):
    # speed sollte zwischen 0.0 und 1.0 liegen
    robot.set_speed(speed)
    return {"speed": speed}

@app.get("/", response_class=HTMLResponse)
async def get_control_page():
    html_content = """
    <!DOCTYPE html>
    <html lang="de">
    <head>
        <meta charset="UTF-8">
        <title>Robot Control Center</title>
        <style>
            body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
                   display: flex; flex-direction: column; align-items: center; 
                   background: #1a1a1a; color: white; min-height: 100vh; margin: 0; padding: 20px; }
            
            .card { background: #2d2d2d; padding: 2rem; border-radius: 20px; 
                   box-shadow: 0 10px 30px rgba(0,0,0,0.5); text-align: center; width: 350px; }
            
            h2 { margin-bottom: 20px; color: #00d4ff; }
            
            /* Slider Styling */
            input[type=range] { width: 100%; margin: 20px 0; cursor: pointer; }
            .val-display { font-size: 2.5rem; font-weight: bold; margin-bottom: 30px; color: #00d4ff; }

            /* Button Grid Styling */
            .button-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; margin-top: 20px; }
            
            button { 
                padding: 12px; border: none; border-radius: 8px; cursor: pointer;
                font-weight: bold; background: #444; color: white; transition: 0.2s;
            }
            
            button:active { transform: scale(0.95); background: #555; }
            
            /* Spezielle Farben für wichtige Buttons */
            .btn-stop { background: #ff4d4d; grid-column: span 2; }
            .btn-stop:active { background: #cc0000; }
            .btn-action { background: #00d4ff; color: #1a1a1a; }
        </style>
    </head>
    <body>
        <div class="card">
            <h2>Control Panel</h2>
            
            <div class="val-display" id="v">0.0</div>
            <input type="range" id="s" min="0" max="1" step="0.001" value="0.0" oninput="updateSpeed(this.value)">
            
            <div class="button-grid">
                <button class="trigger" onclick="trigger('stop')">STOP</button>
                <button onclick="trigger('blink')">Blink</button>
                <button onclick="trigger('horn')">Horn</button>
                <button class="trigger" onclick="trigger('happy')">Happy</button>
                <button onclick="trigger('negative')">Negative</button>
                <button onclick="trigger('attention')">Attention</button>
                <button onclick="trigger('dreamy')">Dreamy</button>
                <button onclick="trigger('error')">Error</button>
            </div>
        </div>

        <script>
            // Geschwindigkeit senden
            async function updateSpeed(val) {
                document.getElementById('v').innerText = parseFloat(val).toFixed(2);
                try {
                    await fetch(`http://127.0.0.1:2026/drive/${val}`);
                } catch(e) { console.error("Drive-Endpoint nicht erreichbar"); }
            }

            // Sound/Aktion triggern
            async function trigger(action) {
                console.log("Triggering:", action);
                try {
                    const response = await fetch(`http://127.0.0.1:2026/sound/${action}`);
                    if (!response.ok) console.error("Server-Fehler");
                } catch(e) { console.error("Sound-Endpoint nicht erreichbar"); }
            }
        </script>
    </body>
    </html>
    """
    return HTMLResponse(content=html_content)


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=2026)
