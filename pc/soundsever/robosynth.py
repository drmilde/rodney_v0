from signalflow import *
import random

class MotorPatch (Patch):
    def __init__(self):
        super().__init__()
        note = self.add_input("note", 60)
        amplitude = self.add_input("amplitude", 0.5)
        gate = self.add_input("gate", 1.0)

        freq = MidiNoteToFrequency(note)
        env = ADSREnvelope(0.001, 0.5, 0.9, 0.2, gate=gate)
        signal = SawOscillator(freq)
        signal = Resample(signal, bit_rate=3)
        filter_env = ADSREnvelope(0.001, 0.1, 0.5, 0.5, gate=gate) ** 4
        filter_env = ScaleLinExp(filter_env, 0, 1, 800, 8000)
        filter_lfo = SineLFO(RandomUniform(3, 8, Impulse(0)), 0.7, 1.3)
        resonance = RandomUniform(0.5, 0.99, Impulse(0))
        signal = SVFilter(signal, SIGNALFLOW_FILTER_TYPE_LOW_PASS, filter_env * filter_lfo, resonance)
        lfo = SineLFO(8, 0.6, 1.0)
        pan = RandomUniform(-0.5, 0.5, Impulse(0))
        output = StereoPanner(signal * env * amplitude * lfo * 0.2, pan)
        output = Resample(output, bit_rate=8)

        self.set_auto_free = True
        self.set_output(output)

class BlinkPatch (Patch):
    def __init__(self):
        super().__init__()
        osc = SineOscillator(1500) 
        env = ASREnvelope(0.01, 0.05, 0.3, curve=2)

        output = osc * env

        self.set_auto_free = True
        self.set_output(output)


class HornPatch (Patch):
    def __init__(self):
        super().__init__()
        # Zweitönige Hupe (Intervall)
        osc1 = SineOscillator(440)
        osc2 = SineOscillator(554.37) # Große Terz
        osc = (osc1 + osc2) 
        gain = 0.3 
        env = ASREnvelope(0.01, 0.3, 0.1)
        
        output = osc * env * gain

        self.set_auto_free = True
        self.set_output(output)

class HappyPatch (Patch):
    def __init__(self):
        super().__init__()

        # Schnelle Zufallsfolge (R2D2-Style)
        clock = Impulse(9)
        counter = Counter(clock=clock)   
        arpeggios = [random.uniform(800, 2000) for i in range(30)]
        sequence = Sequence(arpeggios, clock)

        osc = SquareOscillator(frequency=sequence)
        env = ASREnvelope(0.01, 0.5, 0.01, 1.0, clock*(counter<8))
        gain =  0.2
        signal = osc * env * gain

        #patch.play(delay=i * 0.06)

        # delayed = AllpassDelay(input=patch, 
        #                     delay_time=0.15, 
        #                     feedback=0.1)

        # pan = TriangleLFO(0.1, -1.0, 1.0)
        # output = StereoPanner(delayed, pan) * 0.2
        output = signal

        self.set_auto_free = True
        self.set_output(output)

class StopPatch (Patch):
    def __init__(self):
        super().__init__()

        clock = Impulse(45)
        counter = Counter(clock=clock)   
        freqs = [((30 - i) * 8) for i in range(30)]
        sequence = Sequence(freqs, clock)

        vols = [(((30 - i) / 30.0) / 5.0) for i in range(30)]
        gain = Sequence(vols, clock)


        env = ASREnvelope(0.01, 0.5, 0.02, 1.0, clock*(counter<7))
        osc = SquareOscillator(frequency=sequence) * gain

        output = osc * env 
        self.set_auto_free = True
        self.set_output(output)


class NegativePatch (Patch):
    def __init__(self):
        super().__init__()

        freq = Line(800, 200, 1.7)
        osc = SawOscillator(freq) 
        env =  ASREnvelope(0.1, 0.6, 0.1) * 0.3

        output = osc * env
        self.set_auto_free = True
        self.set_output(output)

class AttentionPatch (Patch):
    def __init__(self):
        super().__init__()

        clock = Impulse(3)
        env = ASREnvelope(0.01, 0.1, 0.5)
        f = Sequence([440, 659.25], clock) # A4 und E5
        osc = SineOscillator(f) * 0.4
 
        output = osc * env
        self.set_auto_free = True
        self.set_output(output)


class ErrorPatch (Patch):
    def __init__(self):
        super().__init__()

        clock = Impulse(50)
        f = Sequence([300, 220], clock)
        osc = SquareOscillator(f)
        env = ASREnvelope(0.05, 0.2, 0.1) * 0.2

        output = osc * env
        self.set_auto_free = True
        self.set_output(output)


class DreamyPatch (Patch):
    def __init__(self):
        super().__init__()

        clock = Impulse(4.5)
        counter = Counter(clock=clock)   
        Am7 = [ 67, 64, 60, 57 ] * 1
        D7 = [ 62, 66, 69, 72] * 1
        arpeggios = Am7 + D7
        sequence = Sequence(arpeggios, clock)
        frequency = MidiNoteToFrequency(sequence)

        oscillator = SquareOscillator(frequency)
        envelope = ASREnvelope(0.1, 0, 0.2, 1.0, clock*(counter<8*2))
        voice = oscillator * envelope
        filtered = SVFilter(voice, "low_pass", 4000, 0.3)
        delayed = AllpassDelay(input=filtered, 
                            delay_time=0.15, 
                            feedback=0.8)

        pan = TriangleLFO(0.1, -1.0, 1.0)
        output = StereoPanner(delayed, pan) * 0.1

        output = output
        self.set_auto_free = True
        self.set_output(output)



class RobotSynth:
    def __init__(self):
        self.graph = AudioGraph()
        self.speed_param = 0
        self.note = (self.speed_param * 60) / 100.0
        self.velocity = 0

        self.motorpatch = MotorPatch()
        self.spec = self.motorpatch.to_spec()
        self.voices = [ None ] * 128
        print(self.spec.to_json())

        self.motor = Patch(self.spec)
        self.motor.set_input("note", self.note)
        self.motor.set_input("amplitude", self.velocity / 127)
        self.motor.auto_free = True
        self.motor.play()

        self.blinkPatch = BlinkPatch()
        self.hornPatch = HornPatch()
        self.happyPatch = HappyPatch()
        self.negativePatch = NegativePatch()
        self.attentionPatch = AttentionPatch()
        self.errorPatch = ErrorPatch()
        self.stopPatch = StopPatch()
        self.dreamyPatch = DreamyPatch()    

        # Dynamischer Parameter für die Geschwindigkeit (0.0 bis 1.0)
        #self.speed_param = PatchParameter("speed", 0.0)
        
        # Drive-Knoten (läuft dauerhaft, aber Lautstärke hängt von speed_param ab)
        #self.drive_osc = SineOscillator(self.speed_param * 400 + 100) * self.speed_param * 0.2
        #self.drive_osc.play()

    def play_blink(self):
        # Kurzer, hoher Piep-Ton
        self.blinkPatch.stop()
        self.blinkPatch = None
        self.blinkPatch = BlinkPatch()          
        self.blinkPatch.play()        

    def play_horn(self):
        # Zweitönige Hupe (Intervall)
        self.hornPatch.stop()
        self.hornPatch = None
        self.hornPatch = HornPatch()
        self.hornPatch.play()

    def play_stop(self):
        # Sequenz wird langsamer und leiser
        self.stopPatch.stop()
        self.stopPatch = None
        self.stopPatch = StopPatch()
        self.stopPatch.play()

    def play_happy(self):
        # Schnelle, fröhliche Arpeggio-Sequenz
        self.happyPatch.stop()
        self.happyPatch = None
        self.happyPatch = HappyPatch()
        self.happyPatch.play()

    def play_negative(self):
        # Gleitender Ton nach unten
        self.negativePatch.stop()
        self.negativePatch = None
        self.negativePatch = NegativePatch()
        self.negativePatch.play()

    def play_attention(self):
        # Intervallsprung nach oben (Quinte)
        self.attentionPatch.stop()
        self.attentionPatch = None
        self.attentionPatch = AttentionPatch()
        self.attentionPatch.play()

    def play_error(self):
        # Intervallsprung nach unten (Dissonant)
        self.errorPatch.stop()
        self.errorPatch = None
        self.errorPatch = ErrorPatch()
        self.errorPatch.play()

    def dreamy(self):
        self.dreamyPatch.stop()
        self.dreamyPatch = None 
        self.dreamyPatch = DreamyPatch()
        self.dreamyPatch.play() 

    def set_speed(self, value: float):
        # Setzt die Geschwindigkeit für den Drive-Sound
        self.speed_param = int(max(0.0, min(1.0, value)) * 100)
        self.note = (self.speed_param * 60) / 100.0
        self.velocity = 10

        if (self.speed_param > 0):
            self.motor.set_input("note", self.note)
            self.motor.set_input("amplitude", self.velocity / 127)
        else:
            self.motor.set_input("note", self.note)
            self.motor.set_input("amplitude", 0)
