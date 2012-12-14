#include "Animation.h"

#include <SFML/OpenGL.hpp>

#include <iostream>

Animation::Animation(const sf::Texture& texture, int nFrames, float fps) : m_texture(texture), m_nFrames(nFrames), m_fps(fps), m_tPlaying(0)
{
}


Animation::~Animation()
{
}

sf::Vector2u Animation::GetSize() const 
{ 
	auto sz = m_texture.getSize(); 
	sz.x /= m_nFrames;
	return sz;
}

void Animation::Update(float tDelta)
{
	m_tPlaying += tDelta;

//			self.offset = (int)((self.timeSinceStart) * self.fps / 1000) * self.get_frame_size().x
//
//			if (self.offset >= self.image.get_width()): # We have gone past the last frame.
//				if (self.loop):
//					self.offset = self.offset % self.image.get_width()  # Wrap around.
}

void Animation::Draw() const
{
	int nFrame = int(m_tPlaying * m_fps) % m_nFrames;
	//if (m_nFrames > 1)
	//	std::cout << m_tPlaying << "," << m_fps << ", " << nFrame << std::endl;
	m_texture.bind();

	auto sz = m_texture.getSize();

	float tL = nFrame / (float)m_nFrames;
	float tR = (nFrame + 1) / (float)m_nFrames;

	glBegin(GL_QUADS);
	glTexCoord2f(tL, 1);
	glVertex3f(-0.5, 0, 0);
	glTexCoord2f(tR, 1);
	glVertex3f(0.5, 0, 0);
	glTexCoord2f(tR, 0);
	glVertex3f(0.5, 1, 0);
	glTexCoord2f(tL, 0);
	glVertex3f(-0.5, 1, 0);
	glEnd();
}

//class Sprite:
//	def __init__(self, path, frames = 1, fps = 1.0, loop = True):
//		self.image = pygame.image.load(path).convert_alpha()
//		self.flipped_image = None
//		self.loop = loop
//		self.fps = fps
//		self.frames = frames
//		
//		self.offset = 0
//		self.timeSinceStart = 0
//
//	def reset(self):
//		self.timeSinceStart = 0
//
//	def start(self):
//		if self.timeSinceStart == -1:
//			self.timeSinceStart = 0
//
//	def stop(self):
//		self.timeSinceStart = -1
//		self.offset = 0       # Reset to the first frame.
//
//	def get_fps(self):
//		return self.fps
//
//	def get_frame_count(self):
//		return self.frames
//
//	def is_playing(self):
//		return self.timeSinceStart >= 0
//
//	def get_frame_size(self):
//		return Vec2(self.image.get_width() / self.frames, self.image.get_height())
//
//	def update(self, timeElapsed) :
//		if (self.timeSinceStart >= 0):   # Playing...
//			self.timeSinceStart += timeElapsed
//
//			# Update the offset based on the elapsed time.
//			self.offset = (int)((self.timeSinceStart) * self.fps / 1000) * self.get_frame_size().x
//
//			if (self.offset >= self.image.get_width()): # We have gone past the last frame.
//				if (self.loop):
//					self.offset = self.offset % self.image.get_width()  # Wrap around.
//				else:
//					Stop()
//
//	# Draw the current frame at <pos>, flipping horizontally if required.
//	def draw(self, screen, pos, flip):
//		if flip:
//			if not self.flipped_image:
//				self.flipped_image = pygame.transform.flip(self.image, True, False)
//			img = self.flipped_image
//			offset = self.image.get_width() - self.offset - self.get_frame_size().x
//		else:
//			img = self.image
//			offset = self.offset
//		
//		screen.blit(img, pos, pygame.Rect(offset, 0, self.get_frame_size().x, self.get_frame_size().y))
