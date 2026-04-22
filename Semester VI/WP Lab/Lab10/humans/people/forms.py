from django import forms
from .models import Human

class HumanForm(forms.ModelForm):
    class Meta:
        model = Human
        fields = '__all__'
