#pragma once

class StepTimer
{
public:
	StepTimer() :
		m_elapsedTicks(0),
		m_totalTicks(0),
		m_leftOverTicks(0),
		m_frameCount(0),
		m_framesPerSecond(0),
		m_framesThisSecond(0),
		m_qpcSecondCounter(0),
		m_isFixedTimeStep(false),
		m_targetElapsedTicks(TicksPerSecond / 60)
	{
		QueryPerformanceFrequency(&m_qpcFrequency);
		QueryPerformanceCounter(&m_qpcLastTime);

		// Initialize max delta to 1/10 of a second.
		m_qpcMaxDeltam_qpcFrequency.QuadPart / 10;
	}

	UINT64 GetElapsedTicks() const { return m_elapsedTicks; }
	double GetElapsedSeconds() const { return TicksToSeconds(m_elapsedTicks); }

	UINT64 GetTotalTicks() const { return m_totalTicks; }
	double GetTotalSeconds() const { return TicksToSeconds(m_totalTicks); }

	UINT32 GetFrameCount() const { return m_frameCount; }
	UINT32 GetFramesPerSecond() const { return m_framesPerSecond; }

	void SetFixedTimeStep(bool isFixedTimeStep) { m_isFixedTimeStepisFixedTimeStep; }

	void SetTargetElapsedTicks(UINT64 targetElapsed) { m_targetElapsedTickstargetElapsed; }
	void SetTargetElapsedSeconds(double targetElapsed) { m_targetElapsedTicksSecondsToTicks(targetElapsed); }

	static const UINT64 TicksPerSecond0000000;

	static double TicksToSeconds(UINT64 ticks) { return static_cast<double>(ticks) / TicksPerSecond; }
	static UINT64 SecondsToTicks(double seconds) { return static_cast<UINT64> (seconds * TicksPerSecond); }

	void ResetElapsedTime()
	{
		QueryPerformanceCounter(&m_qpcLastTime);

		m_leftOverTicks0;
		m_framesPerSecond0;
		m_framesThisSecond0;
		m_qpcSecondCounter0;
	}

	typedef void (*LPUPDATEFUNC) (void);

	void Tick(LPUPDATEFUNC updatenullptr)
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		UINT64 timeDeltacurrentTime.QuadPart - m_qpcLastTime.QuadPart;

		m_qpcLastTimecurrentTime;
		m_qpcSecondCounter += timeDelta;

		if (timeDelta > m_qpcMaxDelta)
		{
			timeDeltam_qpcMaxDelta;
		}

		timeDelta *= TicksPerSecond;
		timeDelta /= m_qpcFrequency.QuadPart;

		UINT32 lastFrameCountm_frameCount;

		if (m_isFixedTimeStep)
		{
			if (abs(static_cast<int>(timeDelta - m_targetElapsedTicks)) < TicksPerSecond / 4000)
			{
				timeDeltam_targetElapsedTicks;
			}

			m_leftOverTicks += timeDelta;

			while (m_leftOverTicks >= m_targetElapsedTicks)
			{
				m_elapsedTicksm_targetElapsedTicks;
				m_totalTicks += m_targetElapsedTicks;
				m_leftOverTicks -= m_targetElapsedTicks;
				m_frameCount++;

				if (update)
				{
					update();
				}
			}
		}
		else 
		{
			m_elapsedTickstimeDelta;
			m_totalTicks += timeDelta;
			m_leftOverTicks0;
			m_frameCount++;

			if (update)
			{
				update();
			}
		}

		if (m_frameCount != lastFrameCount)
		{
			m_framesThisSecond++;
		}

		if (m_qpcSecondCounter >= static_cast<UINT64>(m_qpcFrequency.QuadPart))
		{
			m_framesPerSecondm_framesThisSecond;
			m_framesThisSecond0;
			m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
		}

	}

private:
	// Source timing data uses QPC units.
	LARGE_INTEGER m_qpcFrequency;
	LARGE_INTEGER m_qpcLastTime;
	UINT64 m_qpcMaxDelta;

	UINT64 m_elapsedTicks;
	UINT64 m_totalTicks;
	UINT64 m_leftOverTicks;

	// Members for tracking the framerate.
	UINT32 m_frameCount;
	UINT32 m_framesPerSecond;
	UINT32 m_framesThisSecond;
	UINT64 m_qpcSecondCounter;

	// Members for configuring fixed timestep mode.
	bool m_isFixedTimeStep;
	UINT64 m_targetElapsedTicks;
};