#ifndef PITCH_DETECTION_H
#define PITCH_DETECTION_H

#include <complex>
#include <ffts/ffts.h>
#include <mlpack/core.hpp>
#include <mlpack/methods/hmm/hmm.hpp>
#include <stdexcept>
#include <vector>

/* ignore me plz */
namespace detail
{
std::vector<size_t>
bin_pitches(const std::vector<std::pair<float, float>>);

mlpack::hmm::HMM<mlpack::distribution::DiscreteDistribution>
build_hmm();

void
init_pitch_bins();
} // namespace detail

/*
 * The pitch namespace contains the functions:
 *
 * 	pitch::mpm(data, sample_rate)
 * 	pitch::yin(data, sample_rate)
 * 	pitch::pyin(data, sample_rate)
 * 	pitch::pmpm(data, sample_rate)
 *
 * It will auto-allocate any buffers.
 */
namespace pitch
{

float
yin(const std::vector<float> &, int);

float
mpm(const std::vector<float> &, int);

float
swipe(const std::vector<float> &, int);

/*
 * pyin and pmpm emit pairs of pitch/probability
 */
float
pyin(const std::vector<float> &, int);

float
pmpm(const std::vector<float> &, int);
} // namespace pitch

/*
 * This namespace is useful for repeated calls to pitch for the same size of
 * buffer.
 *
 * It contains the classes Yin and Mpm which contain the allocated buffers
 * and each implement a `pitch(data, sample_rate)` and
 * `probablistic_pitch(data, sample_rate)` method.
 */
namespace pitch_alloc
{

class BaseAlloc
{
  public:
	long nfft;
	std::vector<std::complex<float>> out_im;
	std::vector<float> out_real;
	ffts_plan_t *fft_forward;
	ffts_plan_t *fft_backward;
	mlpack::hmm::HMM<mlpack::distribution::DiscreteDistribution> hmm;

	BaseAlloc(long nfft)
	    : nfft(nfft), out_im(std::vector<std::complex<float>>(nfft)),
	      out_real(std::vector<float>(nfft))
	{
		if (nfft == 0) {
			throw std::bad_alloc();
		}

		fft_forward = ffts_init_1d(nfft, FFTS_FORWARD);
		fft_backward = ffts_init_1d(nfft, FFTS_BACKWARD);
		detail::init_pitch_bins();
		hmm = detail::build_hmm();
	}

	~BaseAlloc()
	{
		ffts_free(fft_forward);
		ffts_free(fft_backward);
	}

  protected:
	void
	clear()
	{
		std::fill(out_im.begin(), out_im.end(), std::complex<float>(0.0, 0.0));
	}
};

/*
 * Allocate the buffers for MPM for re-use.
 * Intended for multiple consistently-sized audio buffers.
 *
 * Usage: pitch_alloc::Mpm ma(1024)
 *
 * It will throw std::bad_alloc for invalid sizes (<1)
 */
class Mpm : public BaseAlloc
{
  public:
	Mpm(long nfft) : BaseAlloc(nfft){};

	float
	pitch(const std::vector<float> &, int);

	float
	probabilistic_pitch(const std::vector<float> &, int);
};

/*
 * Allocate the buffers for YIN for re-use.
 * Intended for multiple consistently-sized audio buffers.
 *
 * Usage: pitch_alloc::Yin ya(1024)
 *
 * It will throw std::bad_alloc for invalid sizes (<2)
 */
class Yin : public BaseAlloc
{
  public:
	int yin_buffer_size;
	std::vector<float> yin_buffer;

	Yin(long nfft)
	    : BaseAlloc(nfft),
		  yin_buffer_size(nfft / 2),
	      yin_buffer(std::vector<float>(yin_buffer_size))
	{
		if (yin_buffer_size == 0) {
			throw std::bad_alloc();
		}
	}

	float
	pitch(const std::vector<float> &, int);

	float
	probabilistic_pitch(const std::vector<float> &, int);
};
} // namespace pitch_alloc

namespace util
{
std::pair<float, float>
parabolic_interpolation(const std::vector<float> &, int);

void
acorr_r(const std::vector<float> &, pitch_alloc::BaseAlloc *);

float
pitch_from_hmm(mlpack::hmm::HMM<mlpack::distribution::DiscreteDistribution>,
    const std::vector<std::pair<float, float>>);
} // namespace util

#endif /* PITCH_DETECTION_H */
