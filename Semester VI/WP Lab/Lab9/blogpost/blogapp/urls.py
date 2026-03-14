from django.urls import path
from blogapp.views import archive, create_blogpost

urlpatterns = [
    path('', archive, name='archive'),
    path('create/', create_blogpost, name='create_blogpost'),
]