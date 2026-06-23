# powershellで
# wsl

# wslで
sudo apt update && \
sudo apt install -y python3 python3-pip python3-venv && \
mkdir -p ~/med-data-analysis && \
cd ~/med-data-analysis && \
python3 -m venv venv && \
source venv/bin/activate && \
python -m pip install --upgrade pip && \
pip install cycler matplotlib numpy opencv-python pandas pydicom Pillow scikit-learn scikit-image seaborn && \
echo "SUCCESS"
