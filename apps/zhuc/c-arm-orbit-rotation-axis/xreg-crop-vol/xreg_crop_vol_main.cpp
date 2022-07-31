//
// Created by zhuch on 2022/7/23.
//

// xreg
#include "xregProgOptUtils.h"
#include "xregITKIOUtils.h"
#include "xregITKCropPadUtils.h"
#include "xregACSVUtils.h"

using namespace xreg;

template <class tPixelType>
void ProcessHelper(const std::string& src_vol_path,
                   const Pt3& center_pt,
                   const Pt3& phys_dims,
                   const std::string& dst_vol_path,
                   std::ostream& vout)
{
  using Vol = itk::Image<tPixelType,3>;

  vout << "reading volume from disk..." << std::endl;
  auto vol = ReadITKImageFromDisk<Vol>(src_vol_path);

  vout << "cropping..." << std::endl;
  vol = CropImageWithBoundBoxPhysPts(vol.GetPointer(), center_pt, phys_dims);

  vout << "writing cropped vol to disk..." << std::endl;
  WriteITKImageToDisk(vol.GetPointer(), dst_vol_path);
}

int main(int argc, char* argv[])
{
  if (true) {
    argv[1] = "-v";
    argv[2] = R"(D:\tmp\xreg_output\saw_bone_01_FC18.nii.gz)";
    argv[3] = R"(D:\data\xreg\c-arm-orbit-rotation-axis\cut_pelvis_roi.acsv)";
    argv[4] = R"(D:\tmp\xreg_output\saw_bone_01_FC18_crop.nii.gz)";

    argc = 5;
  }

  const int kEXIT_VAL_SUCCESS = 0;
  const int kEXIT_VAL_BAD_USE = 1;

  ProgOpts po;

  xregPROG_OPTS_SET_COMPILE_DATE(po);

  po.set_help("Crops a volume using a ROI specified in a .acsv file "
              "(e.g. created in 3D Slicer).");
  po.set_arg_usage("<Input Volume> <ACSV ROI File> <Output Volume>");

  po.set_min_num_pos_args(3);

  po.add("no-ras2lps", ProgOpts::kNO_SHORT_FLAG, ProgOpts::kSTORE_TRUE, "no-ras2lps",
         "Do NOT convert RAS to LPS (or LPS to RAS); Do NOT negate the first and second components.")
          << false;

  try
  {
    po.parse(argc, argv);
  }
  catch (const xreg::ProgOpts::Exception& e)
  {
    std::cerr << "Error parsing command line arguments: " << e.what() << std::endl;
    po.print_usage(std::cerr);
    return kEXIT_VAL_BAD_USE;
  }

  if (po.help_set())
  {
    po.print_usage(std::cout);
    po.print_help(std::cout);
    return kEXIT_VAL_SUCCESS;
  }

  const std::string input_vol_path  = po.pos_args()[0];
  const std::string acsv_roi_path   = po.pos_args()[1];
  const std::string output_vol_path = po.pos_args()[2];

  const bool no_ras2lps = po.get("no-ras2lps");

  std::ostream& vout = po.vout();

  vout << "reading acsv file..." << std::endl;
  const auto roi = ReadROIFromACSV(acsv_roi_path, !no_ras2lps);

  decltype(&ProcessHelper<float>) process_fn = nullptr;

  vout << "determining output pixel type based on input pixel type..." << std::endl;

  itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(
          input_vol_path.c_str(),
          itk::ImageIOFactory::ReadMode);
  imageIO->SetFileName(input_vol_path);
  imageIO->ReadImageInformation();

  switch (imageIO->GetComponentType())
  {
    case itk::ImageIOBase::UCHAR:
      process_fn = &ProcessHelper<unsigned char>;
      break;
    case itk::ImageIOBase::CHAR:
      process_fn = &ProcessHelper<char>;
      break;
    case itk::ImageIOBase::USHORT:
      process_fn = &ProcessHelper<unsigned short>;
      break;
    case itk::ImageIOBase::SHORT:
      process_fn = &ProcessHelper<short>;
      break;
#if 0
      case itk::ImageIOBase::UINT:
    process_fn = &ProcessHelper<unsigned int>;
    break;
  case itk::ImageIOBase::INT:
    process_fn = &ProcessHelper<int>;
    break;
  case itk::ImageIOBase::ULONG:
    process_fn = &ProcessHelper<unsigned long>;
    break;
  case itk::ImageIOBase::LONG:
    process_fn = &ProcessHelper<long>;
    break;
#endif
    case itk::ImageIOBase::FLOAT:
      process_fn = &ProcessHelper<float>;
      break;
    case itk::ImageIOBase::DOUBLE:
      process_fn = &ProcessHelper<double>;
      break;
    case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
    default:
      xregThrow("unknown pixel type!");
      break;
  }

  process_fn(input_vol_path, std::get<0>(roi), std::get<1>(roi),
             output_vol_path, vout);

  vout << "exiting..." << std::endl;

  return kEXIT_VAL_SUCCESS;
}
