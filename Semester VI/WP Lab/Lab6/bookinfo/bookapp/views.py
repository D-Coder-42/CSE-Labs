from django.shortcuts import render

# Create your views here.
def home(request):
    return render(request, 'home.html')

def metadata(request):
    return render(request, 'metadata.html')

def reviews(request):
    return render(request, 'reviews.html')

def publisher(request):
    return render(request, 'publisher.html')
