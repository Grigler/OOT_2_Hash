#include "DataHandler.h" //Contains class header


DataDrawInterface::DataDrawInterface()
{
	m_winWidth = 600;
	m_winHeight = 200;

	m_xPoint = 0;
	m_lastPoint = m_winHeight;

	m_window = SDL_CreateWindow("Analytics", 500, 500, m_winWidth, m_winHeight, SDL_WINDOW_SHOWN);

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_TARGETTEXTURE);
	m_renderTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_winWidth, m_winHeight);
	SDL_SetRenderTarget(m_renderer, m_renderTexture);

	//Set intial state for render texture
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(m_renderer);
	SDL_RenderPresent(m_renderer);
}
DataDrawInterface::~DataDrawInterface()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
}

void DataDrawInterface::PlotFPSPoint(float _fps)
{
	SDL_SetRenderTarget(m_renderer, m_renderTexture);

	m_xPoint++;
	if(m_xPoint > m_winWidth)
	{
		m_xPoint = 1;
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(m_renderer);
	}
	//Each pixel on y represents (win/600)fps 
	int thisPointY = m_winHeight - ((m_winHeight/600.0f)*_fps);
	//Draw this point
	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawPoint(m_renderer, ++m_xPoint, thisPointY);
	//Draw line from this point to last
	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLine(m_renderer, m_xPoint, thisPointY, m_xPoint-1, m_lastPoint);

	//Removing render target to present to copy texture to buffer and present
	SDL_SetRenderTarget(m_renderer, NULL);
	SDL_RenderCopy(m_renderer, m_renderTexture, NULL, NULL);
	SDL_RenderPresent(m_renderer);

	m_lastPoint = thisPointY;
}